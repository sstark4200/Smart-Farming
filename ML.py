import pandas as pd
import tensorflow as tf
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score

# Load the data
df = pd.read_csv('/content/f2.csv')

# Split the data into features (X) and target 👍
X = df.drop('soil_needs_moisture', axis=1).values
y = df['soil_needs_moisture'].values

# Apply PCA to the features
pca = PCA(n_components=0.95)
X = pca.fit_transform(X)

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=0)

# Convert target labels to one-hot encoding
y_train = tf.keras.utils.to_categorical(y_train)
y_test = tf.keras.utils.to_categorical(y_test)

# Build the model
model = tf.keras.Sequential([
    tf.keras.layers.Dense(64, input_dim=X_train.shape[1], activation='relu'),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(y_train.shape[1], activation='softmax')
])

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
history = model.fit(X_train, y_train, epochs=100, validation_split=0.2)

# Evaluate the model on the test set
test_loss, test_accuracy = model.evaluate(X_test, y_test)

# Make predictions on the test set
y_pred = model.predict(X_test).argmax(axis=1)

# Calculate precision
precision = precision_score(y_test.argmax(axis=1), y_pred, average='weighted')

print("Test Accuracy: ", test_accuracy)
print("Precision: ", precision)