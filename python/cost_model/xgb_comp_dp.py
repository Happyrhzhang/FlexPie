# 导入必要的扩展包
import xgboost as xgb
from numpy import array
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.model_selection import train_test_split
import argparse

parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument("--csvname", type=str, default="0")
args = parser.parse_args()

# 读取文件原始数据
data = []
data2=[]
labels = []
labels2 = []

# 读取文件
# 针对每一行数据，根据‘,’分割，转换成列表list
# line_split[8]是标签列
with open(args.csvname, encoding='UTF-8') as fileObject:
    for line in fileObject:
        line_split = line.split(',')
        data2.append(line_split[0])
        data.append(line_split[1:8])
        labels.append(line_split[8])



values = array(data2)
print(values)
# integer encode
label_encoder = LabelEncoder()
integer_encoded = label_encoder.fit_transform(values)
print(integer_encoded)
# binary encode
onehot_encoder = OneHotEncoder(sparse=False)
integer_encoded = integer_encoded.reshape(len(integer_encoded), 1)
onehot_encoded = onehot_encoder.fit_transform(integer_encoded)
print(onehot_encoded)

x=[]
for row,oh in zip(data,onehot_encoded):
    row = [float(x) for x in row]
    row.extend(oh)
    print(row)
    x.append(row)
y = [float(x)/1000000 for x in labels]

# # XGBoost训练过程
#确定训练集 测试集   部分训练部分测试  改为全部训练全部测试
x_train, x_test, y_train, y_test = train_test_split(x, y,
                                                    test_size=0.005,
                                                    random_state=0)

# # 训练模型的参数
params = {
    'booster': 'gbtree',
    'objective': 'reg:gamma',
    'gamma': 0.00001, # 损失函数降低的最小阈值0.2
    'max_depth': 10,  #5
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

#目前最低0.0024  对于3000  depth 5
# 弱学习器的个数，及迭代的次数
num_rounds = 5000  #300
plst = list(params.items())

# 训练模型
# 指定参数，训练数据，弱学习器的个数
xgb_model = xgb.train(plst, dtrain, num_rounds)
xgb_model.save_model('xgb_comp_dp.model')
# 对测试集进行预测
dtest = xgb.DMatrix(x_test)
ans = xgb_model.predict(dtest)
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
