from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime

# Initialize Flask app
app = Flask(__name__)

# Function to create the SQLite database and table
def create_database():
    conn = sqlite3.connect('PlantPulse_DB.db')
    cursor = conn.cursor()

    cursor.execute('''
    CREATE TABLE IF NOT EXISTS Environment (
        Date TEXT NOT NULL,
        Temperature REAL NOT NULL,
        Humidity REAL NOT NULL,
        PH REAL NOT NULL,
        Nitrogen REAL NOT NULL,
        Phosphorus REAL NOT NULL,
        Potassium REAL NOT NULL,
        Overall_Health TEXT
    )
    ''')

    conn.commit()
    conn.close()

@app.route('/')
def home():
    return "PlantPulse Flask App Running"

# Route to handle POST requests from the sensor
@app.route('/sensor-data', methods=['POST'])
def sensor_data():
    # Get data from the POST request (sent by C code)
    data = request.get_json()

    # Extract individual fields
    temperature = data.get('Temperature')
    humidity = data.get('Humidity')
    ph = data.get('PH')
    nitrogen = data.get('Nitrogen')
    phosphorus = data.get('Phosphorus')
    potassium = data.get('Potassium')
    overall_health = data.get('Overall_Health')

    # Insert the sensor data into the SQLite database
    conn = sqlite3.connect('PlantPulse_DB.db')
    cursor = conn.cursor()

    cursor.execute('''
    INSERT INTO Environment (Date, Temperature, Humidity, PH, Nitrogen, Phosphorus, Potassium, Overall_Health)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', (datetime.now(), temperature, humidity, ph, nitrogen, phosphorus, potassium, overall_health))

    conn.commit()
    conn.close()

    return jsonify({"status": "success", "message": "Data inserted successfully!"})

# Call the create_database function to ensure the database is created when the app starts
with app.app_context():
    create_database()

if __name__ == '__main__':
    # Run the Flask app
    app.run(debug=True)

