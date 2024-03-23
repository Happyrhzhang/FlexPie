import xgboost as xgb


def init():
    global xgb_model1
    xgb_model1= xgb.Booster(model_file='xgb_tran_dp.model')
    global xgb_model2
    xgb_model2= xgb.Booster(model_file='xgb_reset_dp.model')
    global xgb_model3
    xgb_model3= xgb.Booster(model_file='xgb_comp_dp.model')

def xgb_tran(feature1,feature2):
    feature_t = [[feature1, feature2]]
    dtest = xgb.DMatrix(feature_t)
    ans = xgb_model1.predict(dtest)
    return ans

def xgb_reset (feature):

    feature_t =[ [feature]]
    dtest = xgb.DMatrix(feature_t)
    ans = xgb_model2.predict(dtest)
    return ans

def xgb_comp (feature1,feature2,feature3,feature4,feature5,feature6,feature7,feature8):

    feature_t = [feature2,feature3,feature4,feature5,feature6,feature7,feature8]
    if feature1 == "Conv1*1":
        values=[1,0]
    elif feature1=="DwConv3*3":
        values=[0,1]
    feature_t.extend(values)
    dtest = xgb.DMatrix([feature_t])
    ans = xgb_model3.predict(dtest)
    return ans


if __name__ == '__main__':
    init()
    # aa=xgb_tran(5.0,438532.0)
    # print(aa)
    # cc=xgb_comp("conv1*1",256,256,7,224,7,224,1)
    # print(cc)

