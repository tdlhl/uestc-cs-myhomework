"""
Reinforcement learning maze example.
Red rectangle:          explorer.
Black rectangles:       hells       [reward = -1].
Yellow bin circle:      paradise    [reward = +1].
All other states:       ground      [reward = 0].
This script is the environment part of this example.
The RL is in RL_brain.py.
View more on my tutorial page: https://morvanzhou.github.io/tutorials/
"""


import numpy as np
import time
import sys    #引入库
if sys.version_info.major == 2:     #读取Python的版本，选择正确模块引入
    import Tkinter as tk
else:
    import tkinter as tk



UNIT = 40   # pixels
MAZE_H = 8  # grid height
MAZE_W = 8  # grid width      #生成窗口初始值


class Maze(tk.Tk, object):
    def __init__(self):
        super(Maze, self).__init__()      #
        self.action_space = ['u', 'd', 'l', 'r']       #角色移动方向
        self.n_actions = len(self.action_space)
        self.title('maze')
        self.geometry('{0}x{1}'.format(MAZE_H * UNIT, MAZE_H * UNIT))      #构建指定大小的二维空间
        self._build_maze()

    def _build_maze(self):
        self.canvas = tk.Canvas(self, bg='white',
                           height=MAZE_H * UNIT,
                           width=MAZE_W * UNIT)

        # create grids                                                  #绘制窗口
        for c in range(0, MAZE_W * UNIT, UNIT):   #c从0到宽度，步长为一个单位
            x0, y0, x1, y1 = c, 0, c, MAZE_H * UNIT
            self.canvas.create_line(x0, y0, x1, y1)
        for r in range(0, MAZE_H * UNIT, UNIT):
            x0, y0, x1, y1 = 0, r, MAZE_W * UNIT, r
            self.canvas.create_line(x0, y0, x1, y1)      #划横线，竖线

        # create origin
        origin = np.array([20, 20])     #一个像素格子的中心，作为原点使用

        # hell
        hell1_center = origin + np.array([UNIT * 6, UNIT*6])
        self.hell1 = self.canvas.create_rectangle(
            hell1_center[0] - 15, hell1_center[1] - 15,
            hell1_center[0] + 15, hell1_center[1] + 15,
            fill='black')
        # hell
        hell2_center = origin + np.array([UNIT*2, UNIT * 2])
        self.hell2 = self.canvas.create_rectangle(
            hell2_center[0] - 15, hell2_center[1] - 15,
            hell2_center[0] + 15, hell2_center[1] + 15,
            fill='black')                                    #分别初始化两个地狱的位置及大小
        hell3_center = origin + np.array([UNIT * 2, UNIT*6])
        self.hell3 = self.canvas.create_rectangle(
            hell3_center[0] - 15, hell3_center[1] - 15,
            hell3_center[0] + 15, hell3_center[1] + 15,
            fill='black')
        hell4_center = origin + np.array([UNIT * 6, UNIT * 2])
        self.hell4 = self.canvas.create_rectangle(
            hell4_center[0] - 15, hell4_center[1] - 15,
            hell4_center[0] + 15, hell4_center[1] + 15,
            fill='black')
        # create oval
        oval_center = origin + UNIT * 4
        self.oval = self.canvas.create_oval(
            oval_center[0] - 15, oval_center[1] - 15,
            oval_center[0] + 15, oval_center[1] + 15,
            fill='yellow')                            #初始化终点位置及大小

        # create red rect    #
        self.rect = self.canvas.create_rectangle(
            origin[0] - 15, origin[1] - 15,
            origin[0] + 15, origin[1] + 15,
            fill='red')                          #初始化角色位置

        # pack all
        self.canvas.pack()

    def reset(self):
        self.update()
        time.sleep(0.5)       #推迟程序运行
        self.canvas.delete(self.rect)
        origin = np.array([20, 20])
        self.rect = self.canvas.create_rectangle(
            origin[0] - 15, origin[1] - 15,
            origin[0] + 15, origin[1] + 15,
            fill='red')
        # return observation
        return self.canvas.coords(self.rect)            #

    def step(self, action):

        s = self.canvas.coords(self.rect)
        base_action = np.array([0, 0])
        if action == 0:   # up
            if s[1] > UNIT:
                base_action[1] -= UNIT
        elif action == 1:   # down
            if s[1] < (MAZE_H - 1) * UNIT:
                base_action[1] += UNIT
        elif action == 2:   # right
            if s[0] < (MAZE_W - 1) * UNIT:
                base_action[0] += UNIT
        elif action == 3:   # left
            if s[0] > UNIT:
                base_action[0] -= UNIT     #对角色移动方向的响应

        self.canvas.move(self.rect, base_action[0], base_action[1])  # move agent    #移动角色

        s_ = self.canvas.coords(self.rect)  # next state           #下一个状态

        # reward function
        if s_ == self.canvas.coords(self.oval):
            reward = 1
            done = True
            s_ = 'terminal'


        elif s_ in [self.canvas.coords(self.hell1), self.canvas.coords(self.hell2),self.canvas.coords(self.hell3),self.canvas.coords(self.hell4)]:
            reward = -1
            done = True
            s_ = 'terminal'
        else:
            reward = 0
            done = False             #评估下一个状态，到终点回报=1，掉入黑洞回报=-1，其它=0

        return s_, reward, done          #返回下一状态，回报值，是否结束

    def render(self):
        time.sleep(0.1)
        self.update()               #更新角色状态
