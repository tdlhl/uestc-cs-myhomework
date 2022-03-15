import matplotlib.pylab as plt
import matplotlib

matplotlib.rcParams['font.sans-serif'] = ['SimHei']
matplotlib.rcParams['font.serif'] = ['SimHei']

arrow_args = dict(arrowstyle="<-")
leafNode = dict(boxstyle="round4", fc="0.8")
decisionNode = dict(boxstyle="sawtooth", fc="0.8")

def getleafs(myTree):
    leafs = 0
    dic_f = list(myTree.keys())[0]
    dic_sec = myTree[dic_f]
    for key in dic_sec.keys():
        if type(dic_sec[key]).__name__ == 'dict':
            leafs += getleafs(dic_sec[key])
        else:
            leafs += 1
    return leafs

def getdepth(myTree):
    depth = 0
    dic_f = list(myTree.keys())[0]
    dic_sec = myTree[dic_f]
    for key in dic_sec.keys():
        if type(dic_sec[key]).__name__ == 'dict':
            depth_t = 1 + getdepth(dic_sec[key])
        else:
            depth_t = 1
        if depth<depth_t:
            depth = depth_t

    return depth

def plot_text(cntrPt, parentPt, txtString):
    cen_y = (parentPt[1] - cntrPt[1]) / 2.0 + cntrPt[1]
    cen_x = (parentPt[0]-cntrPt[0])/2.0 + cntrPt[0]
    Plot.ax1.text(cen_x, cen_y, txtString)
    
def plot_point(nodeTxt, centerPt, parentPt, nodeType):
    Plot.ax1.annotate(nodeTxt, xy=parentPt, xycoords='axes fraction',
                            xytext=centerPt, textcoords='axes fraction',
                            va="center", ha="center", bbox=nodeType, arrowprops=arrow_args)

def plot_tree(myTree, parentPt, nodeTxt):
    leafs = getleafs(myTree=myTree)
    depth = getdepth(myTree=myTree)
    dic_f = list(myTree.keys())[0]
    cntrPt = (plot_tree.xOff + (1.0 + float(leafs))/2.0/plot_tree.totalW, plot_tree.yOff)
    plot_text(cntrPt, parentPt, nodeTxt)
    plot_point(dic_f, cntrPt, parentPt, decisionNode)
    dic_sec = myTree[dic_f]
    plot_tree.yOff = plot_tree.yOff - 1.0/plot_tree.totalD
    for key in dic_sec.keys():
        if isinstance(dic_sec[key], dict):
            plot_tree(dic_sec[key], cntrPt, str(key))
        else:
            plot_tree.xOff = plot_tree.xOff + 1.0/plot_tree.totalW
            plot_point(dic_sec[key], (plot_tree.xOff, plot_tree.yOff), cntrPt, leafNode)
            plot_text((plot_tree.xOff, plot_tree.yOff), cntrPt, str(key))

    plot_tree.yOff = plot_tree.yOff + 1.0/plot_tree.totalD

def Plot(inTree):
    fig = plt.figure(1, facecolor='white')
    fig.clf()
    axprops = dict(xticks=[], yticks=[])
    Plot.ax1 = plt.subplot(111, frameon=False, **axprops)
    plot_tree.totalW = float(getleafs(inTree))
    plot_tree.totalD = float(getdepth(inTree))
    plot_tree.xOff = -0.5/plot_tree.totalW
    plot_tree.yOff = 1.0
    plot_tree(inTree, (0.5, 1.0), '')
    plt.show()
    #plt.savefig("att_4")

if __name__ == '__main__':
    testTree = {'no surfacing': {0: 'no', 1: {'flippers': {0: 'no', 1: 'yes'}}, 3: 'maybe'}}
    Plot(testTree)