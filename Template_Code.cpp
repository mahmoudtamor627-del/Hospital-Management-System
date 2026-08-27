#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== EMERGENCY CASE CLASS ========== //
// Advanced Feature: priority_queue
class EmergencyCase {
private:
    int patientId;
    int severity;

public:
    EmergencyCase(int pid, int s)
    {
        patientId = pid;
        severity = s;
    }


    int getPatientId() const { return patientId; }
    int getSeverity() const { return severity; }

    // Higher severity = higher priority
    bool operator<(const EmergencyCase& other) const {
        return severity < other.severity;
    }
};


// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;

    // Data Structures
    stack<string> medicalHistory;
    queue<string> testQueue;
    vector<string> prescriptions;

    bool isAdmitted;
    RoomType roomType;

    // Advanced Feature: Billing
    double bill;

public:
    // Constructor
    Patient(int pid, string n, int a, string c)
    {
        id = pid;
        name = n;
        age = a;
        contact = c;

        isAdmitted = false;
        bill = 0;
    }

    // ========== ORIGINAL FEATURES ========== //

    void admitPatient(RoomType type)
    {
        if(isAdmitted)
        {
            cout << "Patient is already admitted.\n";
        }
        else
        {
            isAdmitted = true;
            roomType = type;
            medicalHistory.push("Patient admitted to hospital");
            switch(type)
            {
                case GENERAL_WARD:
                bill += 500;
                break;

                case ICU:
                bill += 3000;
                break;

                case PRIVATE_ROOM:
                bill += 1500;
                break;

                case SEMI_PRIVATE:
                bill += 1000;
                break;

            }
        }
    }
    void dischargePatient()
    {
        if(!isAdmitted)
        {
            cout <<  "Patient is not currently admitted.\n";
        }
        else
        {
            isAdmitted = false;
            medicalHistory.push("Patient discharged from hospital");

        }
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }

    void requestTest(string testName)
    {
        testQueue.push(testName);
        medicalHistory.push("Test requested: " + testName);
       cout << "Test '" << testName << "' requested for patient ID " << id << endl;
    }

    string performTest()
    {
        if(testQueue.empty())
        {
            return  "No tests pending";
        }
        else
        {
            string testname = testQueue.front();
            testQueue.pop();

            medicalHistory.push("Test performed: " + testname);
            bill += 300;

            return testname;
        }
    }

    void displayHistory()
    {
        stack<string> st = medicalHistory;
        while(!st.empty())
        {
            cout << st.top() << '\n';
            st.pop();
        }
    }

    int getId()
    {
        return id;
    }
    string getName()
    {
        return name;
    }

    bool getAdmissionStatus()
    {
        return isAdmitted;
    }


    // ========== NEW FEATURES ========== //

    // Medical Tests
    void displayPendingTests()
    {
       cout << "Pending Tests:" << endl;
        queue<string> q = testQueue;
        while(!q.empty())
        {
           cout << " - " << q.front() << '\n';
            q.pop();
        }
    }

    // Prescriptions
    void addPrescription(string medicine)
    {
        prescriptions.push_back(medicine);
        medicalHistory.push( "Prescription added: " + medicine);
        bill += 100;
       cout << "Prescribed '" << medicine << "' to patient ID " << id << endl;
    }

    void displayPrescriptions()
    {
        if(prescriptions.empty())
        {
            cout <<  "No prescriptions.\n";
        }
        else
        {
           cout << "Prescriptions:" << endl;
            for(int i=0; i<prescriptions.size(); i++)
            {
             cout << " - " << prescriptions[i] << '\n';
            }
        }
    }

    // Billing
    void addBill(double amount)
    {
        bill += amount;
    }

    double getBill()
    {
        return bill;
    }

    void displayBill()
    {
        cout << "========== PATIENT BILL ==========\n"
        << "Patient ID: " << id << '\n'
        << "Patient Name: " << name << '\n'
        << "Total Bill: $" << bill << '\n'
        << "==================================\n";

    }

    // Additional Getters
    int getAge()
    {
        return age;
    }

    string getContact()
    {
        return contact;
    }

    RoomType getRoomType()
    {
        return roomType;
    }
};


// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;

    // Queue of patients waiting for doctor
    queue<int> appointmentQueue;

public:
    // Constructor
    Doctor(int did, string n, Department d);

    // ========== ORIGINAL FEATURES ========== //

    void addAppointment(int patientId);
    int seePatient();

    int getId();
    string getName();
    string getDepartment();


    // ========== NEW FEATURES ========== //

    // Display waiting patients
    void displayAppointments();




    // Cancel appointment
    void cancelAppointment(int patientId);

    // Number of waiting patients
    int getAppointmentCount();
};


// ========== HOSPITAL CLASS ========== //
class Hospital {
private:

    // Main collections
    vector<Patient> patients;
    vector<Doctor> doctors;

    // Original emergency queue
    queue<int> emergencyQueue;

    // Advanced emergency queue
    priority_queue<EmergencyCase> priorityEmergencyQueue;

    // Counters
    int patientCounter;
    int doctorCounter;

    // ========== ROOM MANAGEMENT ========== //

    int generalRooms;
    int icuRooms;
    int privateRooms;
    int semiPrivateRooms;


public:

    // Constructor
    Hospital(){
    patientCounter = 1;
    doctorCounter = 1;
    generalRooms = 20;
    icuRooms = 5;
    privateRooms = 10;
    semiPrivateRooms = 10;
}
    // =====================================================
    // ORIGINAL FEATURES
    // ===================================================== //

    int registerPatient(
        string name,
        int age,
        string contact
    )
    {
       int patientId = patientCounter;
       patients.push_back(Patient(patientId, name, age, contact) );
       patientCounter++;
        cout << "Patient registered with ID: " << patientId << endl;
       return patientId;

    }

    int addDoctor(
        string name,
        Department dept
    )
    {
       int doctorId = doctorCounter;
    doctors.push_back(Doctor(doctorId, name, dept));
    doctorCounter++;
    cout << "Doctor added with ID: " << doctorId << endl;
    return doctorId;

    }

  void admitPatient(int patientId, RoomType type)
{
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    if (!isRoomAvailable(type)) {
        cout << "No room available for this room type." << endl;
        return;
    }

    if (patient->getAdmissionStatus()) {
        cout << "Patient is already admitted." << endl;
        return;
    }

    patient->admitPatient(type);
    switch(type) {
        case GENERAL_WARD: generalRooms--; break;
        case ICU:          icuRooms--; break;
        case PRIVATE_ROOM: privateRooms--; break;
        case SEMI_PRIVATE: semiPrivateRooms--; break;
    }
}

    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
         cout << "No emergencies in queue." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handled emergency for patient: " << patientId << endl;
        return patientId;
    }

    void bookAppointment(
        int doctorId,
        int patientId
    )

    {
        Doctor* doctor = findDoctor(doctorId);
    Patient* patient = findPatient(patientId);

    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
    }
    if (patient == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
    }

    if (doctor != nullptr && patient != nullptr) {
        doctor->addAppointment(patientId);
        cout << "Appointment booked for patient " << patientId
             << " with doctor " << doctorId << endl;
    }
    }
    void displayPatientInfo(
        int patientId
    )
    {
        Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    cout << "Patient Information:" << endl;
    cout << "ID: " << patient->getId() << endl;
    cout << "Name: " << patient->getName() << endl;
    cout << "Admission Status: " << (patient->getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
    cout << "Medical History for " << patient->getName() << " (ID: " << patient->getId() << "):" << endl;
    patient->displayHistory();
    }

    void displayDoctorInfo(
        int doctorId
    )
    {
        Doctor* doctor = findDoctor(doctorId);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    cout << "Doctor Information:" << endl;
    cout << "ID: " << doctor->getId() << endl;
    cout << "Name: " << doctor->getName() << endl;
    cout << "Department: " << doctor->getDepartment() << endl;
    }
void searchPatientByName(string name) {
    bool found = false;
    for (Patient& patient : patients) {
        if (patient.getName() == name) {
            found = true;
            cout << "Patient Found:" << endl;
            cout << "ID: " << patient.getId() << endl;
            cout << "Name: " << patient.getName() << endl;
            cout << "Age: " << patient.getAge() << endl;
            cout << "Contact: " << patient.getContact() << endl;
        }
    }
    if (!found) {
        cout << "Patient not found." << endl;
    }
}

    // =====================================================
    // NEW FEATURE 1
    // Find Patient
    // ===================================================== //

    Patient* findPatient(
        int patientId
    )
    {
        for (size_t i=0; i<patients.size(); i++)
        {
            if (patients[i].getId() == patientId)
            {
                return &patients[i];
            }
        }
        return nullptr;
    }


    // =====================================================
    // NEW FEATURE 2
    // Find Doctor
    // ===================================================== //

    Doctor* findDoctor(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 3
    // Search Patient By Name
    // ===================================================== //

   //NEW FEATURE 4
void dischargePatient(int patientId) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    if (patient->getAdmissionStatus()) {
        switch(patient->getRoomType()) {
            case GENERAL_WARD: generalRooms++; break;
            case ICU:          icuRooms++; break;
            case PRIVATE_ROOM: privateRooms++; break;
            case SEMI_PRIVATE: semiPrivateRooms++; break;
        }
        patient->dischargePatient();
        cout << "Patient with ID " << patientId << " has been discharged." << endl;
    } else {
        patient->dischargePatient();
    }
}

// NEW FEATURE 5
void requestPatientTest(int patientId, string testName) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    patient->requestTest(testName);
}

// NEW FEATURE 6
void performPatientTest(int patientId) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
   string result = patient->performTest();
cout << "Performed test for patient ID " << patientId << ": " << result << endl;
}

// NEW FEATURE 7
void displayPatientTests(int patientId) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    patient->displayPendingTests();
}

// NEW FEATURE 8
void prescribeMedicine(int patientId, string medicine) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    patient->addPrescription(medicine);
}

// NEW FEATURE 9
void displayPrescriptions(int patientId) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    patient->displayPrescriptions();
}

//  NEW FEATURE 10
void displayPatientBill(int patientId) {
    Patient* patient = findPatient(patientId);
    if (patient == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }
    patient->displayBill();
}

    // =====================================================
    // NEW FEATURE 11
    // Priority Emergency
    // ===================================================== //

    void addPriorityEmergency(int patientId, int severity)
    {
        if (findPatient(patientId) == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
        }
        if (severity < 1 || severity > 5)
        {
        cout << "Invalid severity.Must be between 1 and 5." << endl;
        return;
        }
        priorityEmergencyQueue.push(EmergencyCase(patientId, severity));
    }


    // =====================================================
    // NEW FEATURE 12
    // Handle Priority Emergency
    // ===================================================== //

    int handlePriorityEmergency()
    {
        if (priorityEmergencyQueue.empty())
        {
           cout << "No priority emergencies." << endl;
            return -1;
        }
        EmergencyCase top = priorityEmergencyQueue.top();
        priorityEmergencyQueue.pop();
        cout << "Handling patient " << top.getPatientId()
         << " with severity " << top.getSeverity() << endl;
        return top.getPatientId();
    }


    // =====================================================
    // NEW FEATURE 13
    // Room Availability
    // ===================================================== //

    bool isRoomAvailable(RoomType type) {
        switch (type) {
        case GENERAL_WARD: return generalRooms > 0;
        case ICU:          return icuRooms > 0;
        case PRIVATE_ROOM: return privateRooms > 0;
        case SEMI_PRIVATE: return semiPrivateRooms > 0;
        default:           return false;
    }
    }


   //(NEW FEATURE 14)
void displayRoomStatus() {
    cout << "========== ROOM STATUS ==========" << endl;
    cout << "General Ward: " << generalRooms << endl;
    cout << "ICU: " << icuRooms << endl;
    cout << "Private Rooms: " << privateRooms << endl;
    cout << "Semi-Private Rooms: " << semiPrivateRooms << endl;
}

//(NEW FEATURE 15)
void displayAllPatients() {
    cout << "========== ALL PATIENTS ==========" << endl;
    for (Patient& patient : patients) {
        cout << "ID: " << patient.getId()
             << " | Name: " << patient.getName()
             << " | Age: " << patient.getAge()
             << " | Status: " << (patient.getAdmissionStatus() ? "Admitted" : "Not Admitted")
             << endl;
    }
}

    // =====================================================
    // NEW FEATURE 16
    // Display All Doctors
    // ===================================================== //


void displayAllDoctors() {
    cout << "========== ALL DOCTORS ==========" << endl;
    for (Doctor& doctor : doctors) {
        cout << "ID: " << doctor.getId()
             << " | Name: " << doctor.getName()
             << " | Department: " << doctor.getDepartment()
             << " | Appointments: " << doctor.getAppointmentCount()
             << endl;
    }
}

    // =====================================================
    // NEW FEATURE 17
    // Display Doctor Appointments
    // ===================================================== //

    void displayDoctorAppointments(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 18
    // Cancel Appointment
    // ===================================================== //

    void cancelAppointment(
        int doctorId,
        int patientId
    );


    // =====================================================
    // NEW FEATURE 19
    // Doctor Sees Next Patient
    // ===================================================== //

    void doctorSeePatient(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 20
    // Hospital Statistics
    // ===================================================== //

    void displayStatistics() {
    int admittedCount = 0;
    double totalBilled = 0;
    for (Patient& patient : patients) {
        if (patient.getAdmissionStatus()) {
            admittedCount++;
        }
        totalBilled += patient.getBill();
    }

    cout << "========== HOSPITAL STATISTICS ==========" << endl;
    cout << "Total Patients: " << patients.size() << endl;
    cout << "Total Doctors: " << doctors.size() << endl;
    cout << "Admitted Patients: " << admittedCount << endl;
    cout << "Waiting Emergencies: " << emergencyQueue.size() << endl;
    cout << "Priority Emergencies: " << priorityEmergencyQueue.size() << endl;
    cout << "Total Generated Bills: $" << totalBilled << endl;
}
};

string departmentToString(Department department) {
    switch (department) {
    case CARDIOLOGY:
        return "Cardiology";
    case NEUROLOGY:
        return "Neurology";
    case ORTHOPEDICS:
        return "Orthopedics";
    case PEDIATRICS:
        return "Pediatrics";
    case EMERGENCY:
        return "Emergency";
    case GENERAL:
    default:
        return "General";
    }
}

Doctor::Doctor(int did, string n, Department d) {
    id = did;
    name = n;
    department = d;
}

void Doctor::addAppointment(int patientId) {
    appointmentQueue.push(patientId);
}

int Doctor::seePatient() {
    if (appointmentQueue.empty()) {
        return -1;
    }

    int patientId = appointmentQueue.front();
    appointmentQueue.pop();
    return patientId;
}

int Doctor::getId() {
    return id;
}

string Doctor::getName() {
    return name;
}

string Doctor::getDepartment() {
    return departmentToString(department);
}

void Doctor::displayAppointments() {
   cout << "Appointments for " << name << ":" << endl;
if (appointmentQueue.empty()) {
    cout << "No patients waiting for doctor " << name << endl;
    return;
}

cout << "Patients waiting for doctor " << name << ":" << endl;
queue<int> tempQueue = appointmentQueue;
while (!tempQueue.empty()) {
    cout << " - Patient ID: " << tempQueue.front() << endl;
    tempQueue.pop();
}
}

void Doctor::cancelAppointment(int patientId) {
    if (appointmentQueue.empty()) {
        cout << "No appointments available." << endl;
        return;
    }

    queue<int> tempQueue;
    bool found = false;

    while (!appointmentQueue.empty()) {
        int currentPatientId = appointmentQueue.front();
        appointmentQueue.pop();

        if (currentPatientId == patientId && !found) {
            found = true;
        }
        else {
            tempQueue.push(currentPatientId);
        }
    }

    appointmentQueue = tempQueue;

    if (found) {
        cout << "Appointment cancelled successfully." << endl;
    }
    else {
        cout << "Appointment not found." << endl;
    }
}

int Doctor::getAppointmentCount() {
    return static_cast<int>(appointmentQueue.size());
}



Doctor* Hospital::findDoctor(int doctorId) {
    for (Doctor& doctor : doctors) {
        if (doctor.getId() == doctorId) {
            return &doctor;
        }
    }

    return nullptr;
}







void Hospital::displayDoctorAppointments(int doctorId) {
    Doctor* doctor = findDoctor(doctorId);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    doctor->displayAppointments();
}

void Hospital::cancelAppointment(int doctorId, int patientId) {
    Doctor* doctor = findDoctor(doctorId);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    doctor->cancelAppointment(patientId);
}

void Hospital::doctorSeePatient(int doctorId) {
    Doctor* doctor = findDoctor(doctorId);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    int patientId = doctor->seePatient();
    if (patientId == -1) {
        cout << "No patients waiting." << endl;
    }
    else {
       cout << doctor->getName()
    << " is seeing patient with ID: " << patientId << endl;
    }
}


// ========== MAIN PROGRAM ========== //
int main() {

    Hospital hospital;


    // =====================================================
    // TEST CASE 1
    // Registering patients
    // ===================================================== //

    int p1 =
        hospital.registerPatient(
            "John Doe",
            35,
            "555-1234"
        );

    int p2 =
        hospital.registerPatient(
            "Jane Smith",
            28,
            "555-5678"
        );

    int p3 =
        hospital.registerPatient(
            "Mike Johnson",
            45,
            "555-9012"
        );


    // =====================================================
    // TEST CASE 2
    // Adding doctors
    // ===================================================== //

    int d1 =
        hospital.addDoctor(
            "Dr. Smith",
            CARDIOLOGY
        );

    int d2 =
        hospital.addDoctor(
            "Dr. Brown",
            NEUROLOGY
        );

    int d3 =
        hospital.addDoctor(
            "Dr. Lee",
            PEDIATRICS
        );


    // =====================================================
    // TEST CASE 3
    // Admitting patients
    // ===================================================== //

    hospital.admitPatient(
        p1,
        PRIVATE_ROOM
    );

    hospital.admitPatient(
        p2,
        ICU
    );

    // Try admitting already admitted patient
    hospital.admitPatient(
        p1,
        SEMI_PRIVATE
    );


    // =====================================================
    // TEST CASE 4
    // Booking appointments
    // ===================================================== //

    hospital.bookAppointment(
        d1,
        p1
    );

    hospital.bookAppointment(
        d1,
        p2
    );

    hospital.bookAppointment(
        d2,
        p3
    );

    // Invalid doctor
    hospital.bookAppointment(
        999,
        p1
    );

    // Invalid patient
    hospital.bookAppointment(
        d1,
        999
    );


    // =====================================================
    // TEST CASE 5
    // Handling medical tests
    // ===================================================== //

    hospital.requestPatientTest(
        p1,
        "Blood Test"
    );

    hospital.requestPatientTest(
        p1,
        "X-Ray"
    );

    hospital.requestPatientTest(
        p1,
        "MRI"
    );

    hospital.displayPatientTests(
        p1
    );

    hospital.performPatientTest(
        p1
    );

    hospital.displayPatientTests(
        p1
    );


    // =====================================================
    // TEST CASE 6
    // Emergency cases
    // ===================================================== //

    hospital.addEmergency(p3);

    hospital.addEmergency(p1);

    int emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    // No more emergencies


    // =====================================================
    // TEST CASE 7
    // Discharging patients
    // ===================================================== //

    hospital.dischargePatient(
        p1
    );


    // =====================================================
    // TEST CASE 8
    // Displaying information
    // ===================================================== //

    hospital.displayPatientInfo(
        p1
    );

    hospital.displayPatientInfo(
        p2
    );

    hospital.displayPatientInfo(
        999
    );


    hospital.displayDoctorInfo(
        d1
    );

    hospital.displayDoctorInfo(
        d2
    );

    hospital.displayDoctorInfo(
        999
    );


    // =====================================================
    // TEST CASE 9
    // Doctor seeing patients
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.doctorSeePatient(
        d1
    );

    hospital.displayDoctorAppointments(
        d1
    );


    // =====================================================
    // TEST CASE 10
    // Search Patient
    // ===================================================== //

    hospital.searchPatientByName(
        "John Doe"
    );

    hospital.searchPatientByName(
        "Unknown Patient"
    );


    // =====================================================
    // TEST CASE 11
    // Prescriptions
    // ===================================================== //

    hospital.prescribeMedicine(
        p1,
        "Paracetamol"
    );

    hospital.prescribeMedicine(
        p1,
        "Antibiotic"
    );

    hospital.displayPrescriptions(
        p1
    );


    // =====================================================
    // TEST CASE 12
    // Patient Billing
    // ===================================================== //

    hospital.displayPatientBill(
        p1
    );

    hospital.displayPatientBill(
        p2
    );


    // =====================================================
    // TEST CASE 13
    // Priority Emergency
    // ===================================================== //

    hospital.addPriorityEmergency(
        p1,
        2
    );

    hospital.addPriorityEmergency(
        p2,
        5
    );

    hospital.addPriorityEmergency(
        p3,
        3
    );

    hospital.addPriorityEmergency(
        p1,
        4
    );


    // =====================================================
    // TEST CASE 14
    // Handle Priority Emergencies
    // ===================================================== //

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();


    // =====================================================
    // TEST CASE 15
    // Room Management
    // ===================================================== //

    hospital.displayRoomStatus();


    // =====================================================
    // TEST CASE 16
    // Display All Patients
    // ===================================================== //

    hospital.displayAllPatients();


    // =====================================================
    // TEST CASE 17
    // Display All Doctors
    // ===================================================== //

    hospital.displayAllDoctors();


    // =====================================================
    // TEST CASE 18
    // Cancel Appointment
    // ===================================================== //

    hospital.cancelAppointment(
        d1,
        p2
    );


    // =====================================================
    // TEST CASE 19
    // More Doctor Appointments
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.displayDoctorAppointments(
        d2
    );


    // =====================================================
    // TEST CASE 20
    // Hospital Statistics
    // ===================================================== //

    hospital.displayStatistics();


    // =====================================================
    // TEST CASE 21
    // Edge Cases
    // ===================================================== //

    Hospital emptyHospital;

    emptyHospital.displayPatientInfo(
        1
    );

    emptyHospital.displayDoctorInfo(
        1
    );

    emptyHospital.handleEmergency();

    emptyHospital.handlePriorityEmergency();

    emptyHospital.searchPatientByName(
        "John Doe"
    );

    emptyHospital.displayAllPatients();

    emptyHospital.displayAllDoctors();

    emptyHospital.displayStatistics();


    return 0;
}
