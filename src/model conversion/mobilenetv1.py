import torch
import torchvision
import torchvision.transforms as transforms
from torch import nn
import onnx


# 普通卷积
class CBN(nn.Module):
    def __init__(self, in_c, out_c, stride=1):
        super(CBN, self).__init__()
        self.conv = nn.Conv2d(in_c, out_c, 3, stride, padding=1, bias=False)
        self.bn = nn.BatchNorm2d(out_c)
        self.relu = nn.ReLU6(inplace=True)

    def forward(self, x):
        x = self.conv(x)
        x = self.bn(x)
        x = self.relu(x)
        return x


# 深度可分离卷积: 深度卷积(3x3x1) + 逐点卷积（1x1xc卷积)
class DwCBN(nn.Module):
    def __init__(self, in_c, out_c, stride=1):
        super(DwCBN, self).__init__()
        # conv3x3x1, 深度卷积，通过步长，只控制是否缩小特征hw
        self.conv3x3 = nn.Conv2d(in_c, in_c, 3, stride, padding=1, groups=in_c, bias=False)
        self.bn1 = nn.BatchNorm2d(in_c)
        self.relu1 = nn.ReLU6(inplace=True)
        # conv1x1xc, 逐点卷积，通过控制输出通道数，控制通道维度的变化
        self.conv1x1 = nn.Conv2d(in_c, out_c, 1, stride=1, padding=0, bias=False)
        self.bn2 = nn.BatchNorm2d(out_c)
        self.relu2 = nn.ReLU6(inplace=True)

    def forward(self, x):
        x = self.conv3x3(x)
        x = self.bn1(x)
        x = self.relu1(x)
        x = self.conv1x1(x)
        x = self.bn2(x)
        x = self.relu2(x)
        return x
#重写cbn函数，pad=4

class MobileNetV1(nn.Module):
    def __init__(self, class_num=1000):
        super(MobileNetV1, self).__init__()
        self.stage1 = torch.nn.Sequential(
            CBN(3, 32, 2),  # 下采样/2
            DwCBN(32, 64, 1)
        )
        self.stage2 = torch.nn.Sequential(
            DwCBN(64, 128, 2),  # 下采样/4
            DwCBN(128, 128, 1)
        )
        self.stage3 = torch.nn.Sequential(
            DwCBN(128, 256, 2),  # 下采样/8
            DwCBN(256, 256, 1)
        )
        self.stage4 = torch.nn.Sequential(
            DwCBN(256, 512, 2),  # 下采样/16
            DwCBN(512, 512, 1),  # 5个
            DwCBN(512, 512, 1),
            DwCBN(512, 512, 1),
            DwCBN(512, 512, 1),
            DwCBN(512, 512, 1),
        )
        self.stage5 = torch.nn.Sequential(
            DwCBN(512, 1024, 2),  # 下采样/32
            DwCBN(1024, 1024, 1)
        )

        # classifier
        self.avg_pooling = torch.nn.AdaptiveAvgPool2d((1, 1))
        self.fc = torch.nn.Linear(1024, class_num, bias=True)

        # self.classifier = torch.nn.Softmax()  # 原始的softmax值
        # torch.log_softmax 首先计算 softmax 然后再取对数，因此在数值上更加稳定。
        # 在分类网络在训练过程中，通常使用交叉熵损失函数（Cross-Entropy Loss）。
        # torch.nn.CrossEntropyLoss 会在内部进行 softmax 操作，因此在网络的最后一层不需要手动加上 softmax 操作。

    def forward(self, x):
        scale1 = self.stage1(x)  # /2
        scale2 = self.stage2(scale1)
        scale3 = self.stage3(scale2)
        scale4 = self.stage4(scale3)
        scale5 = self.stage5(scale4)  # /32. 7x7

        x = self.avg_pooling(scale5)  # (b,1024,7,7)->(b,1024,1,1)
        x = torch.flatten(x, 1)  # (b,1024,1,1)->(b,1024,)
        x = self.fc(x)  # (b,1024,)  -> (b,1000,)
        return x


if __name__ == '__main__':

    m1 = MobileNetV1(class_num=1000)
    torch.save(m1, 'mobilenetv1.pth')
    input_data = torch.randn(1, 3, 224, 224)
    output = m1.forward(input_data)
    print(output.shape)