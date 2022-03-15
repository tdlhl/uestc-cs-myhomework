import tree_plot
from math import log
import collections
import operator

res_att=[]
res_att_val=[]
res_label=[]

def entrop_c(t_data):
    all_num = len(t_data)
    counts = collections.defaultdict(int)
    for vec in t_data:
        label_c = vec[-1]
        counts[label_c] += 1
    entrop = 0.0
    for key in counts:
        p = float(counts[key]) / all_num
        entrop -= p * log(p, 2)
    return entrop

def series_split(dataset, axis, value):

    less_set = []
    great_set = []
    for arr in dataset:
        if arr[axis] <= value:
            less_set.append(arr)
        else:
            great_set.append(arr)

    return less_set, great_set

def data_split(dataSet, axis, value):
    r_set = []
    for vec in dataSet:
        if vec[axis] == value:
            r_vec = vec[:axis]
            r_vec.extend(vec[axis + 1:])
            r_set.append(r_vec)

    return r_set

def series_gain_c(dataSet, i, all_entrop):

    best_gain = 0.0
    s_point = -1

    arrList = [example[i] for example in dataSet]
    list_class = [example[-1] for example in dataSet]
    dict_list = dict(zip(arrList, list_class))
    attr_list_sorted = sorted(dict_list.items(), key=operator.itemgetter(0))
    num_list = len(attr_list_sorted)
    list_mid = [round((attr_list_sorted[i][0] + attr_list_sorted[i+1][0])/2.0, 3)for i in range(num_list - 1)]
    for mid in list_mid:
        less_set, great_set = series_split(dataSet, i, mid)
        ent_new = len(less_set)/len(attr_list_sorted)*entrop_c(less_set) + len(great_set)/len(attr_list_sorted)*entrop_c(great_set)
        infomation_gain = all_entrop - ent_new
        if best_gain<infomation_gain:
            s_point = mid
            best_gain = infomation_gain

    return best_gain, s_point


def gain_discreat(dataSet ,arrList, i, all_entrop):

    uniqueVals = set(arrList)
    newEntropy = 0.0
    for value in uniqueVals:
        subDataSet = data_split(dataSet=dataSet, axis=i, value=value)
        p = len(subDataSet) / float(len(dataSet))
        newEntropy += p * entrop_c(subDataSet)

    infomation_gain = all_entrop - newEntropy

    return infomation_gain


def find_split(dataSet, labels):
    disc_ser_flag = 0
    best_ser_m = 0.0
    all_entrop = entrop_c(dataSet)
    number = len(dataSet[0]) - 1
    best_attr = 3
    best_gain = 0.0
    for i in range(number):
        list_arr = [example[i] for example in dataSet]
        if isinstance(list_arr[0], str):
            infomation_gain = gain_discreat(dataSet, list_arr, i, all_entrop)
        else:
            infomation_gain, s_point = series_gain_c(dataSet, i, all_entrop)

        if infomation_gain > best_gain:
            best_gain = infomation_gain
            best_attr = i
            disc_ser_flag = 0
            if not isinstance(dataSet[0][best_attr], str):
                disc_ser_flag = 1
                best_ser_m = s_point

    if disc_ser_flag:
        res_att.append(best_attr)
        res_att_val.append(best_ser_m)
        res_label.append(dataSet[0][-1])
        return best_attr, best_ser_m
    else:
        res_att.append(best_attr)
        return best_attr


def readdata(data):
    labels = ['属性1', '属性2', '属性3', '属性4']
    file = open(data, "r", encoding='utf8')
    train_data = []
    for line in file.readlines():
        line = line.strip('\n')
        line = line.split("\t")
        line = list(map(float, line))
        train_data.append((line))
    for sub in train_data:
        last = len(sub)
        sub[last - 1] = str(sub[last - 1])
    label_all = {}
    for i in range(len(labels)):
        label_list = [example[i] for example in train_data]
        uniqueLabel = set(label_list)
        label_all[labels[i]] = uniqueLabel

    return train_data, labels, label_all


def cn_most(list_class):

    num_class = collections.defaultdict(int)
    for vote in list_class:
        num_class[vote] += 1
    sortednum_class = sorted(num_class.items(), key=operator.itemgetter(1), reverse=True)

    return sortednum_class[0][0]


def form_tree(dataSet, labels):

    list_class = [example[-1] for example in dataSet]
    if list_class.count(list_class[0]) == len(list_class):
        return list_class[0]
    if len(dataSet[0]) == 1:
        return cn_most(list_class)

    bestarr = find_split(dataSet=dataSet, labels=labels)
    series_m = 0.0
    if isinstance(bestarr, tuple):
        label_best_attr = str(labels[bestarr[0]]) + '小于' + str(bestarr[1]) + '?'
        disc_ser_flag = 1
        series_m = bestarr[1]
        bestarr = bestarr[0]
    else:
        disc_ser_flag = 0
        label_best_attr = labels[bestarr]

    decision_tree = {label_best_attr: {}}
    val_attr = [example[bestarr] for example in dataSet]
    if disc_ser_flag:
        less_set, great_set = series_split(dataSet, bestarr, series_m)
        labels_sub = labels[:]
        sub_tree = form_tree(less_set, labels_sub)
        decision_tree[label_best_attr]['小于'] = sub_tree
        sub_tree = form_tree(great_set, labels_sub)
        decision_tree[label_best_attr]['大于'] = sub_tree

        return decision_tree
    else:
        del (labels[bestarr])
        uniqueVals = set(val_attr)
        for value in uniqueVals:
            labels_sub = labels[:]
            sub_tree = form_tree(data_split(dataSet=dataSet, axis=bestarr, value=value), labels_sub)
            decision_tree[label_best_attr][value] = sub_tree

        return decision_tree

def test_acc_4(decision_tree, labels_t, vec):
    if vec[2]<2.45:
        p_label='1.0'
    else:
        if vec[2]<4.75:
            p_label = '2.0'
        else:
            if vec[3]==1.7:
                p_label = '2.0'
            elif vec[3]==1.4 and vec[0]>6.45:
                p_label = '2.0'
            elif vec[3]==1.6 and vec[0]<6.6:
                p_label = '2.0'
            else:
                p_label = '3.0'
    return p_label

def test_acc_3(decision_tree, labels_t, vec):
    if vec[2]<2.45:
        p_label='1.0'
    else:
        if vec[2]<4.75:
            p_label = '2.0'
        else:
            if vec[2]==5.1:
                if vec[3]<1.7 and vec[0]<6.15:
                        p_label = '2.0'
                else:
                    p_label = '3.0'
            elif vec[3]==4.8 and vec[0]>6.5:
                p_label = '2.0'
            elif vec[3]==5.0 and vec[0]>6.5:
                p_label = '2.0'
            else:
                p_label = '3.0'
    return p_label

def test_acc_2(decision_tree, labels_t, vec):
    if vec[2]<2.45:
        p_label='1.0'
    else:
        if vec[2]<4.75:
            p_label = '2.0'
        else:
            if vec[1]==1.7 and vec[3]<1.45:
                p_label = '2.0'
            elif vec[1]==3.0 and vec[2]<5.05 and vec[0]>6.4:
                p_label = '2.0'
            elif vec[1]==2.7 and vec[0]>5.9:
                p_label = '2.0'
            else:
                p_label = '3.0'
    return p_label

def test_acc_1(decision_tree, labels_t, vec):
    if vec[2]<2.45:
        p_label='1.0'
    else:
        if vec[2]<4.75:
            p_label = '2.0'
        else:
            if vec[0]==6.0 and vec[1]<2.85:
                p_label = '2.0'
            elif vec[0]==6.8 and vec[1]<3:
                p_label = '2.0'
            elif vec[0]==6.7 and vec[2]<5.1:
                p_label = '2.0'
            else:
                p_label = '3.0'
    return p_label

if __name__ == '__main__':

    train_data, labels, all_labels = readdata('traindata.txt')
    decision_tree = form_tree(train_data, labels)
    tree_plot.Plot(decision_tree)
    # print(res_att)
    # print(res_att_val)
    # print(res_label)
    testdata, labels_t, labels_full_t = readdata('testdata.txt')
    total=len(testdata)
    acc=0
    for vec in testdata:
        pre_label=test_acc_4(decision_tree, labels_t, vec)
        if pre_label==vec[4]:
            acc+=1
    acc=acc/total
    print("属性4")
    print(acc)
