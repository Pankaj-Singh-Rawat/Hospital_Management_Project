class User:
    def __init__(self, username, password):
        self.username = username
        self.password = password

class Admin(User):
    def __init__(self, username, password):
        super().__init__(username, password)

class Patient(User):
    def __init__(self, username, password, patient_id):
        super().__init__(username, password)
        self.patient_id = patient_id

class Doctor(User):
    def __init__(self, username, password, doctor_id, specialization):
        super().__init__(username, password)
        self.doctor_id = doctor_id
        self.specialization = specialization

class HospitalManagementSystem:
    def __init__(self):
        self.admins = []
        self.patients = []
        self.doctors = []

    def admin_login(self, username, password):
        for admin in self.admins:
            if admin.username == username and admin.password == password:
                return True
        return False

    def patient_login(self, username, password):
        for patient in self.patients:
            if patient.username == username and patient.password == password:
                return True
        return False

    def doctor_login(self, username, password):
        for doctor in self.doctors:
            if doctor.username == username and doctor.password == password:
                return True
        return False

# Example usage:
hospital = HospitalManagementSystem()

# Creating Admins
admin1 = Admin("admin1", "password1")
hospital.admins.append(admin1)

# Creating Patients
patient1 = Patient("patient1", "password2", "P001")
hospital.patients.append(patient1)

# Creating Doctors
doctor1 = Doctor("doctor1", "password3", "D001", "Cardiology")
hospital.doctors.append(doctor1)

# Testing logins
print(hospital.admin_login("admin1", "password1")) # True
print(hospital.patient_login("patient1", "password2")) # True
print(hospital.doctor_login("doctor1", "password3")) # True
