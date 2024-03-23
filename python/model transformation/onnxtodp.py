# import
import torchvision.models as models
import torch
import onnx
from onnx import numpy_helper
import argparse

parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument("--onnxname", type=str, default="0")
args = parser.parse_args()
model1 = onnx.load(args.onnxname)
# model = onnx.load('new1.onnx')
model = onnx.shape_inference.infer_shapes(model1)

a1=model.functions
a2=model.graph
a3=model.metadata_props
a4=model.opset_import
a5=model.training_info

# print(a1)
# print(a2)    ---a2
# print(a3)
# print(a4)
# print(a5)

b1=a2.value_info
b2=a2.initializer  #存放权重
b3=a2.output
b4=a2.input
b5=a2.node
b6=a2.quantization_annotation
b7=a2.sparse_initializer  #无用

# print(b1)  # -------尺寸在这里
# print(b2)   #存放权重
# print(b3)
# print(b4)
# print(b5)    --b5
# print(b6)
# print(b7)

# c1=b5[0].

#参考
#https://www.zhihu.com/question/386526462
#https://zhuanlan.zhihu.com/p/656090956
#  从b1取尺寸信息   从b5取卷积信息，步长，pad

#定义
nn_number=0
img_height=[]
img_width=[]
input_channel=[]
output_channel=[]
kernelsize=[]
stride=[]
pad=[]
operate=[]

inname=[]
outname=[]
#取卷积等信息 optype=conv 并取name
for i in range(len(model.graph.node)):
    if(model.graph.node[i].op_type=="Conv"):
        inname.append(model.graph.node[i].input[0])
        outname.append(model.graph.node[i].output)
        if model.graph.node[i].attribute[2].ints[0]==3:
            wname=model.graph.node[i].input[1]
            ww=0
            for initializer in model.graph.initializer:
                if initializer.name==wname:
                    W = numpy_helper.to_array(initializer)
                    ww=W.shape[1]
                    break
            if ww==1:
                operate.append("DwConv3*3")
            else:
                operate.append("Conv3*3")
        elif model.graph.node[i].attribute[2].ints[0]==1:
            operate.append("Conv1*1")
        elif model.graph.node[i].attribute[2].ints[0]==5:
            operate.append("Conv5*5")
        elif model.graph.node[i].attribute[2].ints[0]==7:
            operate.append("Conv7*7")

        kernelsize.append(model.graph.node[i].attribute[2].ints[0])
        pad.append(model.graph.node[i].attribute[3].ints[0])
        stride.append(model.graph.node[i].attribute[4].ints[0])

# inname[0][0]
for vall in model.graph.input:
    if vall.name==inname[0]:
        img_height.append(vall.type.tensor_type.shape.dim[2].dim_value)
        img_width.append(vall.type.tensor_type.shape.dim[3].dim_value)
        input_channel.append(vall.type.tensor_type.shape.dim[1].dim_value)
        break

for i in range(0,len(outname)-1):
    for vall in model.graph.value_info:
        if vall.name==outname[i][0]:
            img_height.append(vall.type.tensor_type.shape.dim[2].dim_value)
            img_width.append(vall.type.tensor_type.shape.dim[3].dim_value)
            input_channel.append(vall.type.tensor_type.shape.dim[1].dim_value)
            output_channel.append(vall.type.tensor_type.shape.dim[1].dim_value)
            break

for vall in model.graph.value_info:
    if vall.name==outname[len(outname)-1][0]:
        output_channel.append(vall.type.tensor_type.shape.dim[1].dim_value)
        break

print(inname)
print(outname)

# print(b1[0].name)
# print(b1[0].type.tensor_type.shape.dim[1].dim_value)
# print(model.graph.input)
print(len(img_height),'   ',img_height)
print(len(img_width),'   ',img_width)
print(len(input_channel),'   ',input_channel)
print(len(output_channel),'   ',output_channel)
print(len(operate),'   ',operate)
print(len(kernelsize),'   ',kernelsize)
print(len(pad),'   ',pad)
print(len(stride),'   ',stride)

with open('ir.txt','w') as ir_inp:
    print(len(img_height),file=ir_inp)
    print(img_height,file=ir_inp)
    print(img_width, file=ir_inp)
    print(input_channel, file=ir_inp)
    print(output_channel, file=ir_inp)
    print(operate, file=ir_inp)
    print(kernelsize, file=ir_inp)
    print(pad, file=ir_inp)
    print(stride, file=ir_inp)


# print('11  ',model.graph.node[0])

# for initializer in model.graph.initializer:
#     W= numpy_helper.to_array(initializer)
#     print(initializer.name,W.shape)
#
# print(inname)
# print(outname)




# # print(model1)
# print(model)
graph=model.graph
# att=model.attributes
# print(att)
# print(graph)
node=graph.node
# print(node[0].input)
# print(node)
# elsss=graph.initializer
# print(elsss)
# s1=graph.value_info
# input=graph.input
# print(input)
# output=graph.output
# print(output)
# ----------------------------------------------------------------------------------------------------------------------
