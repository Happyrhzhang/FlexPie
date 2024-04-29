## FlexPie: Accelerate Distributed Inference on Edge Devices with Flexible Model Partition
FlexPie contains a combination of flexible and pie. For edge-based distributed inference applications, the combinatorial optimization is as rich as pie and can be flexibly extended to multiple edge devices.

## Clone Project
```
git clone https://github.com/Happyrhzhang/FlexPie.git
```

## File Structure
The core part includes two modules.
- Cost Model : Generate deployment plan 
- Hardware Test : On-board testing

## Install & Dependencies
- Windows 10
- Compiler: CCS 6.1.0, VS 2019, PyCharm 2021
- Python 3.9
- Pytorch 1.12
- ONNX 1.10.2
- XGBoost 1.7.4
- Sklearn 1.2.1 
- PDK: C6678 1.1.2.6
- Xdctools 3.25.3.72

## Test Example

### Test1 : Generate deployment plan 
S1: On the PC Compiler (PyCharm), compile and run:
```
python ONNXtoIR.py
```
or terminal, run:
```
python ONNXtoIR.py --onnxname "name.onnx"
```
S2: On the PC Compiler (VS), compile and run:
```
./FlexPie/src/dp
```
### Test2 : On-board testing 

## Future Plan
- Support deployment applications of larger scale nodes.
- Extending FlexPie to more edge hardware.
