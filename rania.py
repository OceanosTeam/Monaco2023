import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import random
import math

data= pd.read_csv('datalog.csv')
print(data.shape)
convertion = data.columns.difference(['current_time', 'latitude', 'longitude'])

for column in convertion:
    data[column] = pd.to_numeric(data[column], errors = 'coerce').astype(float)

#motor_zero = data[data['motor_tempMotor'] == 0]
data['motor_tempMotor'] = data['motor_tempMotor'].fillna(0)

#motor_val = data.drop(motor_zero)
motor_temp_list = list(data['motor_tempMotor'])

#data["current_time"] = pd.to_datetime(data["current_time"])
time = data['current_time']
temp = data['motor_tempMotor']
plt.plot(time, temp)
plt.show()

print(motor_temp_list)
sublist_len = len(motor_temp_list)// 20
sublists = [motor_temp_list[i:i+sublist_len] for i in range(0, len(motor_temp_list), sublist_len)]