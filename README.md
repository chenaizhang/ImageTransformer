# ImageTransformer

基于RNAM与STC算法的图像处理软件

## 内容

- [安装](#安装)
- [用户手册](#用户手册)
- [维护者](#维护者)
- [贡献者](#贡献者)

## 安装

[v2.0.1](https://github.com/chenaizhang/ImageTransformer/releases/tag/ImageTransformer)

## 用户手册

<img src="https://github.com/chenaizhang/ImageTransformer/blob/main/image/%E8%BD%AF%E4%BB%B6%E8%BF%90%E8%A1%8C%E7%A4%BA%E6%84%8F%E5%9B%BE.png?raw=true" alt="程序运行示意图" style="zoom: 50%;" />

### 1. 介绍

**软件名称**: ImageTransformer  
**用途**: 基于不同图像处理方法来转换图像。  
**项目地址**: [ImageTransformer](https://github.com/chenaizhang/ImageTransformer/tree/main)  

ImageTransformer 是一个图像转换工具，用户可以通过调整各种参数和选择不同的算法来实现对图像的处理和转换。

### 2. 安装

#### 系统要求
- 操作系统：Windows

#### 安装步骤

1. **克隆项目代码**
   打开命令行工具，输入以下命令克隆项目代码到本地：
   ```bash
   git clone https://github.com/chenaizhang/ImageTransformer.git
   ```

2. **进入项目目录**
   ```bash
   cd ImageTransformer
   ```

3. **安装依赖**
   使用 pip 安装项目所需的依赖：
   ```bash
   pip install -r requirements.txt
   ```

4. **运行软件**
   ```bash
   python main.py
   ```

### 3. 使用说明

#### 3.1 界面概览

启动 ImageTransformer 后，界面主要分为四个区域：原图灰度图、解码后图片、分割示意图和参数设置区域。

#### 3.2 参数设置区域

- **同类快调值**: 输入调整同类快调值（默认：10）。
- **均匀调值**: 输入调整均匀调值（默认：30）。
- **方差调值**: 输入调整方差调值（默认：225）。
- **切割调值**: 输入调整切割调值（默认：1）。
- **编码所用时间**: 显示编码所用的时间。
- **解码所用时间**: 显示解码所用的时间。
- **分块数**: 显示分块的数量。

#### 3.3 算法选择

用户可以在右侧选择合适的算法来进行图像处理：
- RNAM-水平优先
- RNAM-垂直优先
- RNAM-水平竖直中大者优先
- RNAM-对角线与x优先
- RNAM-对角线与y优先
- RNAM-对角线中大者优先
- RNAM-大者优先
- STC

#### 3.4 操作步骤

1. **选择图像**: 点击“图片-导入图片”按钮，选择需要处理的图像。
2. **设置参数**: 根据需要调整参数设置区域的各项值。
3. **选择算法**: 在算法选择区域选择合适的算法。
4. **点击转换**: 点击“图片转换”按钮进行图像处理。
5. **查看结果**: 在“解码后图片”和“分割示意图”区域查看处理后的图像。
6. **保存结果**: 在“图片-导出解码后图片”和“导出分割示意图”按钮，保存结果。

### 4. 常见问题解答

#### Q1: 为什么图像处理后效果不理想？
- 尝试调整参数设置区域的值，特别是同类快调值、均匀调值和方差调值。
- 尝试选择不同的算法进行图像处理，不同算法对不同类型的图像有不同的效果。

#### Q2: 软件运行缓慢怎么办？
- 确保计算机性能足够运行图像处理算法，尤其是大图像处理时。

#### Q3: 遇到错误提示如何解决？
- 提交 issue 到项目的 GitHub 页面，描述详细的错误信息和操作步骤。

如需更多帮助，请访问 [项目地址](https://github.com/chenaizhang/ImageTransformer/tree/main) 获取更多信息和支持。

## 维护者

[@chenaizhang](https://github.com/chenaizhang)

## 贡献者

感谢以下参与项目的人：

[@LM68](https://github.com/LM68)

[@Jane930](https://github.com/Jane930)

[@goodsoupforlove](https://github.com/goodsoupforlove)
