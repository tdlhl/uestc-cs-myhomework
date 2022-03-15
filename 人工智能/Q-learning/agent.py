import numpy as np


class QLearning(object):
    def __init__(self, state_dim, action_dim, cfg):
        self.action_dim = action_dim  # dimension of acgtion
        self.lr = cfg.lr  # learning rate
        self.gamma = cfg.gamma # 衰减系数
        self.epsilon = 0.1
        self.sample_count = 0
        self.Q_table = np.zeros((state_dim, action_dim))  # Q表格

    def predict(self,state):
        Q_list = self.Q_table[state, :]
        Q_max = np.max(Q_list)
        possible_actions = np.where(Q_list == Q_max)[0]
        action = np.random.choice(possible_actions)
        return action

    def choose_action(self, state):
        ####################### 智能体的决策函数，需要完成Q表格方法（需要完成）#######################
        self.sample_count += 1
        if np.random.uniform(0, 1) > self.epsilon:
            action=self.predict(state)
        else:
            action = np.random.choice(self.action_dim)
        return action

    def update(self, state, action, reward, next_state, done):
        ############################ Q表格的更新方法（需要完成）##################################
        Q_old = self.Q_table[state, action]
        if done:
            Q_n = reward
        else:
            Q_n = reward + self.gamma * np.max(self.Q_table[next_state, :])
        self.Q_table[state, action] += self.lr * (Q_n - Q_old)
        #pass

    def save(self, path):
        np.save(path + "Q_table.npy", self.Q_table)

    def load(self, path):
        self.Q_table = np.load(path + "Q_table.npy")
