import json
import hashlib

class HospitalManagementSystem:
    def __init__(self):
        self.user_data_file = "user_data.json"
        self.load_user_data()

    def load_user_data(self):
        try:
            with open(self.user_data_file, 'r') as file:
                self.users = json.load(file)
        except FileNotFoundError:
            self.users = {'admin': {}, 'patient': {}, 'doctor': {}}

    def save_user_data(self):
        with open(self.user_data_file, 'w') as file:
            json.dump(self.users, file)

    def create_id(self, user_type, username, password, details):
        hashed_password = hashlib.sha256(password.encode()).hexdigest()
        self.users[user_type][username] = {'password': hashed_password, **details}
        self.save_user_data()

    def login(self, user_type, username, password=None):
        user_data = self.users.get(user_type, {}).get(username)
        if user_data:
            if password:
                stored_password = user_data.get('password')
                hashed_password = hashlib.sha256(password.encode()).hexdigest()
                if stored_password == hashed_password:
                    return user_data
                else:   
                    print("Incorrect password.")
                    return None
            else:
                return user_data
        else:
            print("User not found. Do you want to sign up?")
            choice = input("Enter 'yes' to sign up or any other key to cancel: ")
            if choice.lower() == 'yes':
                name = input("Enter your name: ")
                email = input("Enter your email: ")
                password = input("Enter your password: ")
                self.create_id(user_type, username, password, {'name': name, 'email': email})
                print("Sign up successful. You can now login.")
                return self.users[user_type][username]
            else:
                print("Sign up canceled.")
                return None

# Sample usage
hms = HospitalManagementSystem()

# Login or Sign up
user_type = input("Enter user type (admin/patient/doctor): ")
username = input("Enter username: ")

user_details = hms.login(user_type, username)
if user_details:
    if 'password' in user_details:
        password = input("Enter password: ")
        user_details = hms.login(user_type, username, password)
    if user_details:
        print("Login successful.")
        print("User details:", user_details)

