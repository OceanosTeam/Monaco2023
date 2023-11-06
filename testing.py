from datetime import date
from datetime import datetime
import os
import shutil
import random 
import math 
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestRegressor
from sklearn.impute import SimpleImputer
from sklearn.impute import KNNImputer


# read csv & calculate size
data = pd.read_csv('test_endurance.csv')
print(data)
print('data type: '+str(type(data)))

# convert to list
datalist = data.values.tolist()
rows = len(datalist) # 20839
columns = len(datalist[0]) # 23

# turn it to np.array
datalist = np.array(datalist)
print('datalist type: '+ str(type(datalist)))
print('rows: '+str(rows))
print('columns: '+str(columns))

# choose motor column
np.set_printoptions(threshold=np.inf)
motor = datalist[:,12]
print(type(motor))
header = motor[0]
print(type(header))

indexes = [] # keeps indexes where we have headers
for i in range(rows-1):
    if motor[i]==header:
        indexes.append(i)
        motor[i]='0' # replaces headers with '0'
    if motor[i]==' ':
        motor[i]='0'

print(indexes)

motor = motor.astype(float)

for i in range(rows): 
    if motor[i]>92:
        motor[i] = round(random.uniform(91,92),1)

motor = np.where(motor == 0, np.nan, motor)
print(motor)
#motor = pd.Series(motor)

#-------------------------------------------

#-------------------------------------------

motor = pd.Series(motor)
motor.plot()

# Display the plot
plt.show()
'''
# motor temprature is 12
datalist = data.values.tolist()
rows = len(datalist)
columns = len(datalist[0])
print(rows)
print(columns)

print(type(datalist))

'''

''''
final=[]
for i in range(rows):
    final.append(datalist[i][12])

for i in range(rows):
    if final[i]==' ':
    #if isinstance(datalist[i][12], float) == False: #finds not a number data
        final[i]= 0
        datalist[i][12] = 0 
        avg_list = []
        #avg_array = [0]
        #count = 0
        try:
            for x in range(i-100,i+100):
                if final[i] != 0 and final[i] != 'motor_tempMotor':
                    #count = count + 1
                    avg_list.append(float(final[i]))
                    #appended_array = np.append(avg_array, [final[i]], axis=0)
                    #print(appended_array)
                    #if count>9:
                        #final[i]=sum(avg_list)/len(avg_list)
                        #break
                elif final[i] == 0 and final[i] != 'motor_tempMotor':
                    final[i]=sum(avg_list)/len(avg_list)
                    break
            print(avg_list)
        except:
            pass
print(final)
gamw = []
for i in range(rows):
    #print(datalist[i][12])
    if final[i] != 'motor_tempMotor':
        gamw.append(float(final[i])-float(datalist[i][12]))

print(sum(gamw))
'''