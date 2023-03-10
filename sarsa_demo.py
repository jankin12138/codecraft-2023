from sarsa import GridWorld
from sarsa import SarsaAgent

if __name__ == '__main__':
    env = GridWorld()
    agent = SarsaAgent(env)
    agent.train()  # 训练智能体
    agent.run()  # 运行智能体并将路径输出到文件

# 测试脚本执行过程：
# 1. 创建一个 5x5 的网格世界
# 2. 创建一个 SARSA 智能体，并初始化 Q 值表、学习率、折扣率和探索率
# 3. 在环境中训练智能体，直到收敛为止
# 4. 运行智能体，并记录其移动路径
# 5. 将路径输出到一个名为 output.txt 的文本文件中，每行一个状态
