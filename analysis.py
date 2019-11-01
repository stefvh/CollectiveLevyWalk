import math

def squared_distance(x1, y1, x2, y2):
    return math.sqrt((x1-x2)**2 + (y1-y2)**2)

with open('levy_data.txt') as f:
    data = f.readlines()

d = {}

for line in data[1:]:
    data_split = line[:-1].split('\t')
    f_x1 = float(data_split[2])
    f_x2 = float(data_split[3])
    if data_split[0] not in d:
        d[data_split[0]] = [(f_x1, f_x2)]
    else:
        d[data_split[0]].append((f_x1, f_x2))

c = 0

for i,robot1 in enumerate(d.keys()):
    for j, robot2 in enumerate(d.keys()):
        if j>i:
            for t1 in d.get(robot1):
                for t2 in d.get(robot2):
                    (x1, y1) = t1
                    (x2, y2) = t2
                    if squared_distance(x1,y1,x2,y2) <= 0.0025:
                        c += 1

print(c)