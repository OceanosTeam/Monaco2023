import numpy as np
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

# Sample list with missing values
motor = [35, 40, 42, None, 38, None, 45, 41, None, 36]

# Convert the list to a numpy array
motor = np.array(motor)

# Generate indices for missing values
missing_indices = np.where(motor == None)[0]

# Generate feature array for training
X = np.arange(len(motor)).reshape(-1, 1)

# Remove missing values from the target variable
y = motor[motor != None]

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train a Random Forest Regressor
model = RandomForestRegressor(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Predict the motor temperatures for the missing values
X_missing = missing_indices.reshape(-1, 1)
y_missing_predicted = model.predict(X_missing)

# Fill in the missing values with the predicted temperatures
motor[missing_indices] = y_missing_predicted

# Evaluate the model's performance on the test set
y_pred = model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
print(f"Mean Squared Error: {mse}")

# Print the updated motor temperatures list
print("Updated Motor:", motor.tolist())
