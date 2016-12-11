# 算法期末--矩阵相乘

### 待处理

1. assert在release模式下仍然有可能影响性能。
2. 矩阵规模超过2万时会导致内存（约5G）不够用。



## 传统算法

### 优化方案

1. 提高cache命中率，初步方案是采用转至矩阵的方法

## Strassen_algorithm

### 优化方案

1. 当矩阵规模小于25x25的时候，可以考虑直接调用传统算法

## 多线程并行运算

参考openMP，vs自带这个库。

[openMP VS2015](https://msdn.microsoft.com/en-us/library/tt15eb9t.aspx)

