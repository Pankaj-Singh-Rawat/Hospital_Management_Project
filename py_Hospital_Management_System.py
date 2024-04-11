import hashlib
import json
import random
import string
import sys

#-----------------------------------------------------------------------

class HospitalManagementSystem:
    def __init__(self):
        self.user_data_file = "user_data.json"
        self.load_user_data()
        self.current_user_username = None

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
                    self.current_user_username = username 
                    return user_data, user_type, username
                else:   
                    print("Incorrect password.")
                    return None, None, None
            else:
                return user_data, user_type, username
        else:
            print("User not found. Do you want to sign up?")
            choice = input("Enter 'yes' to sign up or any other key to cancel: ")
            if choice.lower() == 'yes':
                name = input("Enter your name: ")
                email = input("Enter your email: ")
                password = input("Enter your password: ")
                self.create_id(user_type, username, password, {'name': name, 'email': email})
                print("Sign up successful. You can now login.")
                return self.users[user_type][username], user_type ,username
            else:
                print("Sign up canceled.")
                return None, None

#-----------------------------------------------------------------------
            
    def admin_menu(self):
        while True:
            print("Admin Menu:")
            print("1. View Hospital list")
            print("2. Add Patient")
            print("3. Delete Patient")
            print("4. Add Doctor")
            print("5. Delete Doctor")
            print("6. View All User Data")  # Added option to view all user data
            print("7. Exit")
        
            option = input("Enter your choice: ")
            if option == '1':
                self.view_hospital_list()
            elif option == '2':
                self.add_patient()
            elif option == '3':
                self.delete_patient()
            elif option == '4':
                self.add_doctor()
            elif option == '5':
                self.delete_doctor()
            elif option == '6':
                self.view_all_user_data()   
            elif option == '7':
                print("Exiting admin menu.")
                return
            else:
                print("Invalid option.")
            
            choice = input("Do you want to see the admin menu again? (yes/no): ")
            if choice.lower() != 'yes':
                print("Exiting admin menu.")
                return

    def view_hospital_list(self):
        print("All Hospital under admin access are: ")
        print("1. Pavan Hospital")
        print("2. Rini Hospital")
        print("3. Priyanshu Hospital")

    def add_patient(self):
        username = input("Enter username: ")
        password = input("Enter password: ")
        name = input("Enter Name: ")
        email = input("Enter Email: ")

        hashed_password = hashlib.sha256(password.encode()).hexdigest()

        with open(self.user_data_file, 'r+') as file:
            data = json.load(file)
            patient_data = {
                'password': hashed_password, 
                'name': name,
                'email': email
            }
            data['patient'][username] = patient_data
            file.seek(0)
            json.dump(data, file)
        print(f"Patient '{username}' added successfully.")       

    def delete_patient(self):
        username = input("Enter the username of the patient to delete: ")
        if username in self.users['patient']:
            del self.users['patient'][username]
            self.save_user_data()
            print(f"Patient '{username}' deleted successfully.")
        else:
            print(f"Patient with username '{username}' not found.")

    def add_doctor(self):
        username = input("Enter username: ")
        password = input("Enter password: ")
        name = input("Enter Name: ")
        email = input("Enter Email: ")

        hashed_password = hashlib.sha256(password.encode()).hexdigest()

        with open(self.user_data_file, 'r+') as file:
            data = json.load(file)
            doctor_data = {
                'password': hashed_password, 
                'name': name,
                'email': email
            }
            data['doctor'][username] = doctor_data
            file.seek(0)
            json.dump(data, file)
        print(f"Doctor '{username}' added successfully.")       

    def delete_doctor(self):
        username = input("Enter the username of the doctor to delete: ")
        if username in self.users['doctor']:
            del self.users['doctor'][username]
            self.save_user_data()
            print(f"Doctor '{username}' deleted successfully.")
        else:
            print(f"Doctor with username '{username}' not found.")

    def view_all_user_data(self):
        print("All User Data:")
        for user_type, users in self.users.items():
            print(f"User Type: {user_type}")
            for username, details in users.items():
                print(f"Username: {username}")
                for key, value in details.items():
                    print(f"{key.capitalize()}: {value}")
                print()
            print()
        

#-----------------------------------------------------------------------
            
    def patient_menu(self):
        while True:
            print("Patient Menu:")
            print("1. View Room & Bed number")
            print("2. View my medicines")
            print("3. View my report")
            print("4. Exit")

            option = input("Enter your choice: ")
            if option == '1':
                self.room_and_bed_number()
            elif option == '2':
                self.patient_medicine()
            elif option == '3':
                self.patient_report()
            elif option == '4':
                print("Exiting patient menu.")
                return
            else:
                print("Invalid option.")

            choice = input("Do you want to see the patient menu again? (yes/no): ")
            if choice.lower() != 'yes':
                print("Exiting patient menu.")
                return

    def room_and_bed_number(self):
        print("Room Number:", random.randint(1, 100))
        print("Bed:", random.choice(string.ascii_uppercase))

    def patient_medicine(self):
        print("Your prescribed medicines are:")
        current_patient_username = self.current_user_username  
        if current_patient_username in self.users['patient']:
            patient_data = self.users['patient'][current_patient_username]
            if 'doctor' in patient_data:
                doctor_username = patient_data['doctor']
                if doctor_username in self.users['doctor']:
                    doctor_data = self.users['doctor'][doctor_username]
                    if 'medicines' in doctor_data:
                        print(", ".join(doctor_data['medicines']))
                    else:
                        print("No medicines prescribed by your doctor.")
                else:
                    print("Your doctor's data not found.")
            else:
                print("You are not assigned to any doctor.")
        else:
            print("Patient data not found.")

    def patient_report(self):
        print("Your reports are:")
        current_patient_username = self.current_user_username
        if current_patient_username in self.users['patient']:
            patient_data = self.users['patient'][current_patient_username]
            if 'reports' in patient_data:
                for report in patient_data['reports']:
                    print(report)
            else:
                print("No reports available.")
        else:
            print("Patient data not found.")

#-----------------------------------------------------------------------
    
    def doctor_menu(self):
        while True:
            print("Doctor Menu:")
            print("1. View Patient Reports")
            print("2. ADD Patient Medicine")
            print("3. ADD Patient Report")
            print("4. View Another Patient Data")
            print("5. Exit")
            option = input("Enter your choice: ")

            if option == '1':
                self.view_patient_report()
            elif option == '2':
                self.add_patient_medicine()
            elif option == '3':
                self.add_patient_report()
            elif option == '4':
                self.view_another_patient_data()
            elif option == '5':
                print("Exiting doctor menu.")
                return
            else:
                print("Invalid option. Please try again.")

    def view_patient_report(self):
        while True:
            patient_username = input("Enter the username of the patient whose data you want to view (or type 'exit' to return to the main menu): ")
            if patient_username.lower() == 'exit':
                print("Returning to the main menu.")
                break  
            if patient_username in self.users['patient']:
                
                patient_data = self.users['patient'][patient_username]
                print("Patient Data:")
                print("Username:", patient_username)
                print("Name:", patient_data['name'])
                print("Email:", patient_data['email'])
                if 'report' in patient_data:
                    print("Report:", patient_data['report'])
                if 'medicines' in patient_data:
                    print("Medicines:", ', '.join(patient_data['medicines']))
                print()
            else:
                print(f"Patient with username '{patient_username}' not found.")
                
    def add_patient_medicine(self):
        doctor_username = input("Enter the username of the doctor who prescribed the medicine: ")
        patient_username = input("Enter the username of the patient: ")
        medicine = input("Enter the medicine to add: ")

        if doctor_username in self.users['doctor'] and patient_username in self.users['patient']:
            patient_data = self.users['patient'][patient_username]

            if 'doctors' not in patient_data:
                patient_data['doctors'] = {}

            patient_data['doctors'].setdefault(doctor_username, []).append(medicine)
            self.save_user_data()
            print(f"Medicine '{medicine}' added for patient '{patient_username}' by doctor '{doctor_username}'.")
        else:
            print("Doctor or patient not found.")

    def patient_medicine(self):
        print("Your prescribed medicines are:")
        current_patient_username = self.current_user_username
        if current_patient_username in self.users['patient']:
            patient_data = self.users['patient'][current_patient_username]
            if 'doctors' in patient_data:
                for doctor_username, medicines in patient_data['doctors'].items():
                    print(f"Prescribed by doctor '{doctor_username}': {', '.join(medicines)}")
            else:
                print("No medicines prescribed for you.")
        else:
            print("Patient data not found.")

    def add_patient_report(self):
        patient_username = input("Enter the username of the patient to add report: ")
        report = input("Enter the report to add: ")
        if patient_username in self.users['patient']:
           
            patient_data = self.users['patient'][patient_username]
            patient_data.setdefault('reports', []).append(report) 
            self.save_user_data()
            print(f"Report added for patient '{patient_username}'.")
        else:
           print(f"Patient with username '{patient_username}' not found.")

    def view_another_patient_data(self):
        while True:
            patient_username = input("Enter the username of the patient whose data you want to view (or type 'exit' to return to the main menu): ")
            if patient_username.lower() == 'exit':
                print("Returning to the main menu.")
                break  
            if patient_username in self.users['patient']:
                
                patient_data = self.users['patient'][patient_username]
                print("Patient Data:")
                print("Username:", patient_username)
                print("Name:", patient_data['name'])
                print("Email:", patient_data['email'])
                if 'report' in patient_data:
                    print("Report:", patient_data['report'])
                if 'medicines' in patient_data:
                    print("Medicines:", ', '.join(patient_data['medicines']))
                print()
            else:
                print(f"Patient with username '{patient_username}' not found.")

#-----------------------------------------------------------------------
                
hms = HospitalManagementSystem()

user_type = input("Enter user type (admin/patient/doctor): ")
username = input("Enter username: ")

user_details, user_type, current_username = hms.login(user_type, username)
if user_details and user_type and current_username:
    if 'password' in user_details:
        password = input("Enter password: ")
        user_details, user_type, current_username = hms.login(user_type, username, password)
    if user_details and user_type and current_username:
        print("Login successful.")
        if user_type == 'admin':
            hms.admin_menu()
        
        elif user_type == 'patient':
            hms.patient_menu()

        elif user_type == 'doctor':
            
            hms.doctor_menu()
        sys.exit()


