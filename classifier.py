import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

# Load data and preprocess
data = pd.read_csv('D:/feeds.csv')
data['created_at'] = pd.to_datetime(data['created_at'])
data['EMG_derivative'] = np.gradient(data['EMG'])

# Grouping and feature extraction
group_window_size = pd.Timedelta(minutes=10.5)
sliding_window_size = 10
grouped_data = []
start_time = data['created_at'].min()
end_time = data['created_at'].max()
current_start = start_time
while current_start < end_time:
    current_end = current_start + group_window_size
    group = data[(data['created_at'] >= current_start) & (data['created_at'] < current_end)]
    grouped_data.append(group)
    current_start = current_end

feature_matrices = []
for group in grouped_data:
    features = []
    for i in range(len(group) - sliding_window_size + 1):
        window = group.iloc[i:i + sliding_window_size]
        acc_x_mean = window['Acc_x'].mean()
        acc_y_mean = window['Acc_y'].mean()
        emg_derivative_mean = window['EMG_derivative'].mean()
        features.append([acc_x_mean, acc_y_mean, emg_derivative_mean])
    feature_matrices.append(np.array(features))

# Prepare dataset for training (first six groups)
X = []
y = []
for i, matrix in enumerate(feature_matrices[:-1]):
    for feature in matrix:
        X.append(feature)
        y.append(i)

X = np.array(X)
y = np.array(y)

# Splitting dataset
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=0)

# Training RandomForest Classifier
classifier = RandomForestClassifier(n_estimators=100, random_state=0)
classifier.fit(X_train, y_train)

# Evaluate the classifier
y_pred = classifier.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print(accuracy)
