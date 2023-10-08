import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from scipy.stats import norm
import sys



def plot_data_distribution(data,bins=20,msg='',save_dir=None):
    
    # 计算均值和方差
    mean = np.mean(data)
    variance = np.var(data)
    
    # 绘制数据的直方图和正态分布拟合曲线
    sns.set(style="whitegrid",font_scale=2)  # 设置Seaborn样式
    # 绘制数据分布图并添加正态分布拟合曲线,对数据进行归一化处理
    ax = sns.histplot(data, kde=False, color="skyblue", label="Data", stat='density', bins=bins)  # 绘制数据分布图
    sns.lineplot(x=np.linspace(np.min(data), np.max(data), 100), y=norm.pdf(np.linspace(np.min(data), np.max(data), 100), mean, np.sqrt(variance)), color='red',alpha=0.5, label="Normal Distribution")  # 绘制正态分布拟合曲线
    for l in ax.lines:
        l.set_linewidth(3)


    # 显示均值和方差信息
    plt.title(f'{msg}: Mean={mean:.2f}, Variance={variance:.2f}')
    
    # 添加样本数量的文本标签
    sample_size = len(data)
    plt.text(0.2, 0.9, f'Sample Size: {sample_size}', ha='center', va='center', transform=plt.gca().transAxes, bbox=dict(facecolor='white', alpha=0.5))
    
    # 设置坐标轴线宽
    ax.spines['top'].set_linewidth(2)
    ax.spines['right'].set_linewidth(2)
    ax.spines['bottom'].set_linewidth(2)
    ax.spines['left'].set_linewidth(2)

    # 设置坐标轴刻度大小
    ax.tick_params(axis='both', which='major', labelsize=20)


    
    # 显示图例
    # plt.legend()
    
    # 显示图形
    plt.gcf().set_size_inches(10, 8)
    plt.show()

    if save_dir:
        plt.savefig(save_dir+msg+'.png')


def parse(raw_data):
    # raw data = '[[1,2,3],[4,5,6],[7,8,9]]'
    raw_data = raw_data.replace('[','').replace(']','').split(',')
    data = [int(i) for i in raw_data]
    return data

# 示例用法
if __name__ == "__main__":
    # sys.argv[1] = '[[1,2,3],[4,5,6],[7,8,9]]'
    data = parse(sys.argv[1])
    
    # 调用函数进行计算、拟合和绘图
    plot_data_distribution(data)
