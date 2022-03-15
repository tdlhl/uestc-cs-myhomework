"""
Sarsa is a online updating method for Reinforcement learning.
Unlike Q learning which is a offline updating method, Sarsa is updating while in the current trajectory.
You will see the sarsa is more coward when punishment is close because it cares about all behaviours,
while q learning is more brave because it only cares about maximum behaviour.
"""

from maze_env import Maze
from RL_brain import SarsaTable
import time

def update():
    start = time.clock()
    cs=-1
    for episode in range(100):
        # initial observation
        observation = env.reset()                 #初始化观测值
        end=time.clock()
        cs=cs+1
        print(str(end-start),cs)             #输出运行时间，第几次（第0次为初始化，不计入游戏）
        start = time.clock()

        # RL choose action based on observation
        action = RL.choose_action(str(observation))       #选择行动

        while True:
            # fresh env
            env.render()

            # RL take action and get next observation and reward
            observation_, reward, done = env.step(action)            #从环境获得观测值，回报

            # RL choose action based on next observation
            action_ = RL.choose_action(str(observation_))                               #选择下一步的行动

            # RL learn from this transition (s, a, r, s, a) ==> Sarsa
            RL.learn(str(observation), action, reward, str(observation_), action_)        #更新学习策略

            # swap observation and action
            observation = observation_
            action = action_            #开始下一步

            # break while loop when end of this episode
            if done:
                break         #跳出循环的条件：环境返回done的值为真

    # end of game
    print('game over')
    env.destroy()

if __name__ == "__main__":
    env = Maze()
    RL = SarsaTable(actions=list(range(env.n_actions)))

    env.after(100, update)
    env.mainloop()