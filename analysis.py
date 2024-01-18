import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data = pd.read_csv('D:/feeds.csv')

data['created_at'] = pd.to_datetime(data['created_at'])

#data['EMG_derivative'] = np.gradient(data['EMG'])

time_window = '10.5T'  

grouped_emg_data = data.groupby(pd.Grouper(key='created_at', freq=time_window))

plt.figure(figsize=(15, 8))

colors = plt.cm.viridis(np.linspace(0, 1, grouped_emg_data.ngroups))

for (key, group), color in zip(grouped_emg_data, colors):
    plt.plot(group['created_at'], group['EMG'], label=key, color=color)

plt.xlabel('Time')
plt.ylabel('EMG Reading')
plt.title('EMG Readings')
plt.xticks(rotation=45)
#plt.legend(title='Time Window Start', bbox_to_anchor=(1.05, 1), loc='upper left')
plt.grid(True)
plt.tight_layout()
plt.show()
