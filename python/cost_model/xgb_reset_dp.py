# 导入必要的扩展包
import xgboost as xgb
from xgboost import plot_importance
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument("--csvname", type=str, default="0")
args = parser.parse_args()


# 读取文件原始数据
data = []
# data1=[]
labels = []
labels2 = []

# 读取文件
# 针对每一行数据，根据‘,’分割，转换成列表list
# line_split[1]是标签列
with open(args.csvname, encoding='UTF-8') as fileObject:
    for line in fileObject:
        line_split = line.split(',')
        data.append(line_split[0:1])
        labels.append(line_split[1])


# 构建样本数据X，y
# 先对样本数据进行数据类型的转换
x=[]
for row in data:
    row = [float(x) for x in row]
    # print(row)
    x.append(row)

y = [float(x)*1000 for x in labels]

# # XGBoost训练过程

#确定训练集 测试集   部分训练部分测试  改为全部训练全部测试
x_train, x_test, y_train, y_test = train_test_split(x, y,
                                                    test_size=0.05,
                                                    random_state=0)

# # 训练模型的参数
params = {
    'booster': 'gbtree',
    'objective': 'reg:gamma',
    'gamma': 0.0002, # 损失函数降低的最小阈值
    'max_depth': 7,  #5
    'lambda': 3,   #3
    'subsample': 0.7,
    'colsample_bytree': 0.7,
    'min_child_weight': 3,
    'silent': 1,
    'eta': 0.1, #0.1
    'seed': 1000,
    'nthread': 4,
}

# 把训练特征数据，训练标签数据，进行矩阵的转化
dtrain = xgb.DMatrix(x_train, y_train)

# 弱学习器的个数，及迭代的次数
num_rounds = 300  #300
plst = list(params.items())

# 训练模型
# 指定参数，训练数据，弱学习器的个数
xgb_model = xgb.train(plst, dtrain, num_rounds)
xgb_model.save_model('xgb_reset_dp.model')

# 对测试集进行预测
dtest = xgb.DMatrix(x_test)
print(x_train)
ans = xgb_model.predict(dtest)
print(ans)
print(y_train)
n=0
sum=0
sumb=0
t = dict(zip(ans,y_test))
for a,b in t.items():
    n=n+1
    sum=sum+abs(b-a)
    sumb=sumb+abs((b-a)/b)
    print("预测值:",a,"实际值:",b)
sum=sum/n
sumb=sumb/n
print("平均误差:\n",sum,"平均百分比误差",sumb)
# 平均误差:
#  0.0005733748116928144 平均百分比误差 0.004470333339876439
#
