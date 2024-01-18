import pandas as pd
import numpy as np
from datetime import timedelta

file_path = 'D:/feeds.csv'
data = pd.read_csv(file_path)

data['created_at'] = pd.to_datetime(data['created_at'])

data['roll_angle'] = np.arctan2(data['Acc_y'], data['Acc_x']) * 180 / np.pi

data['EMG_derivative'] = np.gradient(data['EMG'])

window_size = timedelta(minutes=10.5)

start_time = data['created_at'].iloc[0]

groups = []
current_group = []
current_end_time = start_time + window_size

# Loop through each row to assign groups
for index, row in data.iterrows():
    if row['created_at'] <= current_end_time:
        current_group.append(row)
    else:
        groups.append(pd.DataFrame(current_group))
        current_group = [row]
        current_end_time = row['created_at'] + window_size

if current_group:
    groups.append(pd.DataFrame(current_group))

# Storing each group in a separate matrix
group_matrices = [group.reset_index(drop=True) for group in groups]

# Print the entries for the first group
#print(group_matrices[0])

rolling_window_size = 10

# Extract features
features_matrices = [group[['Acc_x', 'Acc_y', 'EMG_derivative']]
                     .rolling(window=rolling_window_size)
                     .agg(['mean', 'var'])
                     .dropna() 
                     for group in group_matrices]

# Display the first group's feature matrix
#print(features_matrices[0])

