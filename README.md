# get-cnn-feature

## Requirement

### caffe
### opencv

## Structure

```
—————./
|————get-roi-from-face
|————Parameter.txt
|————newdata/
||————bruce/
|||————pic1.jpg
|||————pic2.jpg
||————gary/
|||————pic1.jpg
|||————pic2.jpg
|————Result-newdata/
||————Miss.txt
||————bruce/
|||————pic1.jpg
|||————pic2.jpg
|————model/
```

## LFW benchmark SOP

### Step0 training
```
將原始影像截出我們所期望的ROI
```
### Step1 training
```
將 normalize 成 192x192 眼距 40 的 training images
放置根目錄自定義資料夾中
例如，"./dataset/001/pic1.jpg"
```
### Step2 training
```
執行 get-cnn-feature，得到 feature vector.txt & label.txt
```
### Step3 training
```
丟 TXT 給 Gary 去 train bayesian model
```
### Step4 testing
```
執行get-benchmark-roc，拿 train 好的 cnn model 及 bayes model 來 test 結果並存成 roc.TXT
```
### Step5 testing
```
將 roc.TXT 丟給 LFW 提供的 matlab code 去算 ROC 曲線
```












