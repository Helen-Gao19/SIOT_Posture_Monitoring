import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Load your data
data = pd.read_csv('D:/feeds.csv')
data['created_at'] = pd.to_datetime(data['created_at'])

# Calculate roll angles
data['roll_angle'] = np.arctan2(data['Acc_y'], data['Acc_x']) * 180 / np.pi

# Grouping and plotting
start_time = data['created_at'].iloc[0]
window = pd.Timedelta(minutes=10.5)

plt.figure(figsize=(15, 8))

for i in range(6):
    window_start = start_time + i * window
    window_end = start_time + (i + 1) * window
    group = data[(data['created_at'] >= window_start) & (data['created_at'] < window_end)]
    plt.plot(group['created_at'], group['roll_angle'], label=f'Group {i+1}')

plt.xlabel('Time')
plt.ylabel('Roll Angle (degrees)')
plt.title('Roll Angle')
plt.legend()
plt.xticks(rotation=45)
plt.show()
