#include <iostream>
#include <string>
#include <iomanip>
#include <mysql.h>

using namespace std;

MYSQL* conn;
string currentUserId = "";

// 학생 정보 구조체
struct user_info {
    string year; // 입학년도
    string id;   // 학번
    string name; // 이름
    string password; // 비밀번호
    string subject; // 전공
};

// 교수(관리자) 정보 구조체
struct admin_info {
    string year; // 입학년도
    string id;   // 학번
    string name; // 이름
    string password; // 비밀번호
    string subject; // 전공
};

int main_menu();
bool signup();
user_info signUp_user();
admin_info signUp_admin();
string input_id();

bool studentLogin();
void handleStudentMenu();
int student_menu();
void displayEnrollmentStatus();
void withdrawCourse();
int getEnrollmentLimit(const string& courseId);
void changePassword();
bool verifyCurrentPassword(const string& currentPassword);

bool adminLogin();
void handleAdminMenu();
int admin_menu();
void addStudent();
void openCourse();
void deleteCourse();
void displayAllCourses();
void enrollInCourse(const string& studentId, const string& courseId);
void enrollCourse(const string& studentId, const string& courseId);
bool canEnroll(const string& courseId);

// 메인 메뉴 함수
int main_menu() {
    // 메뉴 표시
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1. 회원 가입" << endl;
    cout << "2. 학생 로그인" << endl;
    cout << "3. 관리자 로그인" << endl;
    cout << "0. 종료" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    int stat;
    cout << ">> ";
    cin >> stat;

    return stat;
}

// 회원 가입 함수
bool signup() {
    string part;
    cout << "학생인가요? 교수 및 관리자 인가요?";
    cin >> part;

    if (part == "학생") {
        signUp_user();  // 학생 가입 함수 호출
        return true;    // 가입 성공
    }
    else if (part == "교수" || part == "관리자") {
        signUp_admin();  // 교수/관리자 가입 함수 호출
        return true;     // 가입 성공
    }

    cout << "잘못된 값 입력으로 메인 메뉴로 돌아갑니다." << endl;
    return false;  // 가입 실패
}

// 학생 가입 함수
user_info signUp_user() {
    cout << "--------------------------------------------------------------------------------" << endl;
    user_info new_user;

    // 학번 입력
    cout << "입학년도(4자리): ";
    cin >> new_user.year;

    // 년도 길이 확인
    if (new_user.year.length() != 4) {
        cout << "올바른 년도를 입력해주세요." << endl;
        return new_user;
    }

    // 12345678을 추가하여 code 생성
    string code = new_user.year + "12345678";

    string input_code;
    // 사용자에게 입력을 받음
    cout << "인증코드를 입력하세요: ";
    cin >> input_code;

    // 코드 비교
    if (input_code != code) {
        cout << "인증코드가 틀렸습니다. 다시 시도해주세요." << endl;
        return new_user;
    }

    cout << "인증코드가 올바릅니다. 차례대로 개인정보를 입력해주세요." << endl;

    // 이름 입력
    cout << "영어 이름을 입력하세요: ";
    cin >> new_user.name;

    new_user.id = new_user.year + "A" + input_id();  // 학번 생성

    // 비밀번호 입력
    cout << "비밀번호를 입력하세요: ";
    cin >> new_user.password;

    // 전공 입력
    cout << "전공을 입력하세요: ";
    cin >> new_user.subject;

    // 회원가입 완료 메시지 출력
    cout << "회원가입이 완료되었습니다." << endl;
    cout << "< 입력한 정보 >" << endl;
    cout << "입학년도: " << new_user.year << endl;
    cout << "학번: " << new_user.id << endl;
    cout << "비밀번호: " << new_user.password << endl;
    cout << "전공: " << new_user.subject << endl;

    // 데이터베이스에 회원 정보 삽입
    string insert_query = "INSERT INTO student_table (student_id, username, password, department) VALUES ('" + new_user.id + "', '" + new_user.name + "', '" + new_user.password + "', '" + new_user.subject + "')";
    if (mysql_query(conn, insert_query.c_str()) == 0) {
        cout << "회원 정보를 데이터베이스에 성공적으로 삽입했습니다." << endl;
    }
    else {
        cout << "회원 정보를 데이터베이스에 삽입하는데 실패했습니다. 오류 메시지: " << mysql_error(conn) << endl;
    }

    return new_user;
}

// 교수(관리자) 가입 함수
admin_info signUp_admin() {
    cout << "--------------------------------------------------------------------------------" << endl;
    admin_info new_user;

    // 학번 입력
    cout << "입학년도(4자리): ";
    cin >> new_user.year;

    // 년도 길이 확인
    if (new_user.year.length() != 4) {
        cout << "올바른 년도를 입력해주세요." << endl;
        return new_user;
    }

    // 87654321을 추가하여 code 생성
    string code = new_user.year + "87654321";

    string input_code;
    // 사용자에게 입력을 받음
    cout << "인증코드를 입력하세요: ";
    cin >> input_code;

    // 코드 비교
    if (input_code != code) {
        cout << "인증코드가 틀렸습니다. 다시 시도해주세요." << endl;
        return new_user;
    }

    cout << "인증코드가 올바릅니다. 차례대로 개인정보를 입력해주세요." << endl;

    // 이름 입력
    cout << "영어 이름을 입력하세요: ";
    cin >> new_user.name;

    new_user.id = new_user.year + "A" + input_id();  // 학번 생성

    // 비밀번호 입력
    cout << "비밀번호를 입력하세요: ";
    cin >> new_user.password;

    // 전공 입력
    cout << "전공을 입력하세요: ";
    cin >> new_user.subject;

    // 회원가입 완료 메시지 출력
    cout << "회원가입이 완료되었습니다." << endl;
    cout << "< 입력한 정보 >" << endl;
    cout << "입학년도: " << new_user.year << endl;
    cout << "학번: " << new_user.id << endl;
    cout << "비밀번호: " << new_user.password << endl;
    cout << "전공: " << new_user.subject << endl;

    // 데이터베이스에 회원 정보 삽입
    string insert_query = "INSERT INTO admin_table (professor_id, username, password, department) VALUES ('" + new_user.id + "', '" + new_user.name + "', '" + new_user.password + "', '" + new_user.subject + "')";
    if (mysql_query(conn, insert_query.c_str()) == 0) {
        cout << "회원 정보를 데이터베이스에 성공적으로 삽입했습니다." << endl;
    }
    else {
        cout << "회원 정보를 데이터베이스에 삽입하는데 실패했습니다. 오류 메시지: " << mysql_error(conn) << endl;
    }

    return new_user;
}

// 아이디 입력 함수
string input_id() {
    int num;
    string id_result;

    while (true) {
        cout << "아이디로 사용될 숫자 4자리를 입력해주세요: ";
        cin >> num;

        if (cin.fail() || num > 9999) {
            cout << "올바른 숫자 4자리를 입력해주세요." << endl;
            // 오류 발생한 입력 제거
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break; // 입력이 올바를 때 반복문 탈출
        }
    }

    // 아이디를 문자열로 변환
    if (num < 10) {
        id_result = "000" + to_string(num);
    }
    else if (num < 100) {
        id_result = "00" + to_string(num);
    }
    else if (num < 1000) {
        id_result = "0" + to_string(num);
    }
    else {
        id_result = to_string(num);
    }

    return id_result;
}

// 학생 로그인 함수
bool studentLogin() {
    int maxAttempts = 3;
    int attempts = 0;
    string id;
    string password;

    while (attempts < maxAttempts) {
        cout << "학생 ID: ";
        cin >> id;
        cout << "학생 PW: ";
        cin >> password;

        // 데이터베이스에서 입력받은 ID와 일치하는 정보 조회
        string query = "SELECT * FROM student_table WHERE student_id='" + id + "' AND password='" + password + "'";

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* result = mysql_store_result(conn);

            // 결과가 존재하는 경우 (ID와 비밀번호가 일치하는 경우)
            if (result && mysql_num_rows(result) > 0) {
                MYSQL_ROW row = mysql_fetch_row(result);

                // ID가 위치하는 컬럼 번호에 따라 수정 (예: 0은 첫 번째 컬럼)
                currentUserId = row[0];

                cout << "로그인 성공!" << endl;
                handleStudentMenu();  // 학생 메뉴 처리 함수 호출
                return true;
            }
            else {
                attempts++;
                cout << "로그인 실패. ID 또는 비밀번호를 확인하세요." << endl;
            }
        }
        else {
            cout << "로그인 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
            main_menu();
            return false;
        }
    }

    cout << "로그인 시도 횟수 초과.\n메인 메뉴로 돌아갑니다." << endl;
    main_menu();
    return false;
}

// 학생 메뉴 처리 함수
void handleStudentMenu() {
    int user = -1;

    while (user != 5) {  // 5가 로그아웃에 해당하는 메뉴 번호입니다.
        user = student_menu();
        string enroll_id;

        switch (user) {
        case 1:
            // 수강 신청 함수 호출
            displayAllCourses();
            cout << "과목 코드를 입력해주세요: ";
            cin >> enroll_id;
            enrollCourse(currentUserId, enroll_id);
            break;
        case 2:
            // 수강 현황 함수 호출
            displayEnrollmentStatus();
            break;
        case 3:
            // 수강 철회 함수 호출
            displayEnrollmentStatus();
            withdrawCourse();
            break;
        case 4:
            // 비밀번호 변경 함수 호출
            changePassword();
            break;
        case 5:
            cout << "로그아웃 되었습니다." << endl;
            break;
        case 0:
            cout << "프로그램을 종료합니다." << endl;
            mysql_close(conn);
            exit(0);
        default:
            cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
            break;
        }
    }
}

// 학생 메뉴 함수
int student_menu() {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1. 수강 신청" << endl;
    cout << "2. 수강 현황" << endl;
    cout << "3. 수강 철회" << endl;
    cout << "4. 비밀번호 변경" << endl;
    cout << "5. 로그아웃" << endl;
    cout << "0. 종료" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    int stat;
    cout << ">> ";
    cin >> stat;

    return stat;
}

// 수강 현황을 출력하는 함수
void displayEnrollmentStatus() {
    string query = "SELECT course_table.course_id, course_name FROM course_table JOIN enroll_table ON course_table.course_id = enroll_table.course_id WHERE student_id='" + currentUserId + "'";

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);

        if (result) {
            int numFields = mysql_num_fields(result);
            MYSQL_ROW row;

            cout << setw(15) << "[ 과목 코드 ]" << setw(20) << "[ 과목 이름 ]" << endl;
            cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;

            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < numFields; ++i) {
                    cout << setw(15) << row[i];
                }
                cout << endl;
            }
        }
        else {
            cout << "수강 현황을 가져오는 데 실패했습니다. 오류: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "수강 현황을 질의하는 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
    }
}

// 수강 철회 함수
void withdrawCourse() {
    string withdraw_id;

    cout << "철회할 과목 코드를 입력하세요: ";
    cin >> withdraw_id;

    string withdrawQuery = "DELETE FROM enroll_table WHERE student_id='" + currentUserId + "' AND course_id='" + withdraw_id + "'";

    if (mysql_query(conn, withdrawQuery.c_str()) == 0) {
        int numAffectedRows = mysql_affected_rows(conn);

        if (numAffectedRows > 0) {
            cout << "수강 철회가 성공적으로 처리되었습니다." << endl;
        }
        else {
            cout << "수강 철회가 실패했습니다. 해당하는 수강 신청이 없습니다." << endl;
        }
    }
    else {
        cout << "수강 철회 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
    }
}

// 강의 등록 한도를 가져오는 함수
int getEnrollmentLimit(const string& courseId) {
    string getEnrollmentLimitQuery = "SELECT enrollment_limit FROM course_table WHERE course_id='" + courseId + "'";

    if (mysql_query(conn, getEnrollmentLimitQuery.c_str()) == 0) {
        MYSQL_RES* limitResult = mysql_store_result(conn);

        if (limitResult && mysql_num_rows(limitResult) > 0) {
            MYSQL_ROW limitRow = mysql_fetch_row(limitResult);
            return atoi(limitRow[0]);
        }
        else {
            cout << "등록 한도를 검색하는 데 실패했습니다. 오류: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "등록 한도를 질의하는 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
    }

    return -1;  // 오류를 나타냄
}

// 학생 비밀번호 변경 함수
void changePassword() {
    string currentPassword;
    string newPassword;

    cout << "현재 비밀번호를 입력하세요: ";
    cin >> currentPassword;

    // 입력한 현재 비밀번호를 확인하는 함수 호출
    if (!verifyCurrentPassword(currentPassword)) {
        cout << "현재 비밀번호가 일치하지 않습니다." << endl;
        return;
    }

    cout << "새로운 비밀번호를 입력하세요: ";
    cin >> newPassword;

    // 데이터베이스에 새로운 비밀번호를 업데이트하는 쿼리
    string updateQuery = "UPDATE student_table SET password='" + newPassword + "' WHERE student_id='" + currentUserId + "'";

    if (mysql_query(conn, updateQuery.c_str()) == 0) {
        cout << "비밀번호가 성공적으로 변경되었습니다." << endl;
    }
    else {
        cout << "비밀번호 변경 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
    }
}

// 현재 비밀번호 확인 함수
bool verifyCurrentPassword(const string& currentPassword) {
    // 입력한 현재 비밀번호가 데이터베이스에 일치하는지 확인하는 쿼리
    string query = "SELECT * FROM student_table WHERE student_id='" + currentUserId + "' AND password='" + currentPassword + "'";

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);

        if (result && mysql_num_rows(result) > 0) {
            return true;  // 비밀번호 일치
        }
    }

    return false;  // 비밀번호 불일치
}

// 관리자 로그인 함수
bool adminLogin() {
    int maxAttempts = 3;
    int attempts = 0;
    string id;
    string password;

    while (attempts < maxAttempts) {
        cout << "관리자 ID: ";
        cin >> id;
        cout << "관리자 PW: ";
        cin >> password;

        // 데이터베이스에서 입력받은 ID와 비밀번호가 일치하는지 확인하는 쿼리
        string query = "SELECT * FROM admin_table WHERE professor_id='" + id + "' AND password='" + password + "'";

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* result = mysql_store_result(conn);

            // 결과가 존재하는 경우 (ID와 비밀번호가 일치하는 경우)
            if (result && mysql_num_rows(result) > 0) {
                cout << "로그인 성공!" << endl;
                handleAdminMenu();  // 관리자 메뉴 처리 함수 호출
                return true;
            }
            else {
                attempts++;
                cout << "로그인 실패. ID 또는 비밀번호를 확인하세요." << endl;
            }
        }
        else {
            cout << "로그인 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
            main_menu();
            return false;
        }
    }

    cout << "로그인 시도 횟수 초과.\n메인 메뉴로 돌아갑니다." << endl;
    main_menu();
    return false;
}

// 관리자 메뉴 처리 함수
void handleAdminMenu() {
    int user = -1;

    while (user != 4) {  // 4가 로그아웃에 해당하는 메뉴 번호입니다.
        user = admin_menu();

        switch (user) {
        case 1:
            // 학생 추가 함수 호출
            addStudent();
            break;
        case 2:
            // 강의 개설 함수 호출
            openCourse();
            break;
        case 3:
            // 강의 삭제 함수 호출
            deleteCourse();
            break;
        case 4:
            cout << "로그아웃 되었습니다." << endl;
            break;
        case 0:
            cout << "프로그램을 종료합니다." << endl;
            mysql_close(conn);
            exit(0);
        default:
            cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
            break;
        }
    }
}

// 관리자 메뉴 출력 함수
int admin_menu() {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "1. 학생 추가" << endl;
    cout << "2. 강의 개설" << endl;
    cout << "3. 강의 삭제" << endl;
    cout << "4. 로그아웃" << endl;
    cout << "0. 종료" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    int stat;
    cout << ">> ";
    cin >> stat;

    return stat;
}

void addStudent() {
    string studentyear;
    string num;
    string studentId;
    string studentName;
    string studentPassword;
    string studentDepartment;

    cout << "입학년도(4자리): ";
    cin >> studentyear;

    if (studentyear.length() != 4) {
        cout << "올바른 년도를 입력해주세요." << endl;
        return addStudent();
    }

    studentId = studentyear + "A" + input_id();

    // 중복 체크: 입력한 학번이 이미 존재하는지 확인
    string duplicateCheckQuery = "SELECT * FROM student_table WHERE student_id='" + studentId + "'";

    if (mysql_query(conn, duplicateCheckQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);

        // 결과가 존재하는 경우 (중복된 학번이 존재하는 경우)
        if (result && mysql_num_rows(result) > 0) {
            cout << "이미 존재하는 학번입니다. 다른 학번을 입력해주세요." << endl;
            return;  // 중복된 학번이면 함수 종료
        }
    }
    else {
        cout << "중복 체크 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
        return;  // 중복 체크 중 오류 발생하면 함수 종료
    }

    cout << "영어 이름을 입력하세요: ";
    cin >> studentName;

    cout << "비밀번호를 입력하세요: ";
    cin >> studentPassword;

    cout << "전공을 입력하세요: ";
    cin >> studentDepartment;

    cout << "회원가입이 완료되었습니다." << endl;
    cout << "< 입력한 정보 >" << endl;
    cout << "입학년도: " << studentyear << endl;
    cout << "학번: " << studentId << endl;
    cout << "비밀번호: " << studentPassword << endl;
    cout << "과: " << studentDepartment << endl;

    // 데이터베이스에 학생 정보 삽입
    string insertQuery = "INSERT INTO student_table (student_id, username, password, department) VALUES ('" + studentId + "', '" + studentName + "', '" + studentPassword + "', '" + studentDepartment + "')";

    if (mysql_query(conn, insertQuery.c_str()) == 0) {
        cout << "학생 정보가 성공적으로 추가되었습니다." << endl;
    }
    else {
        cout << "학생 정보 추가 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
    }
}

// 강의를 개설하는 함수
void openCourse() {
    string courseId;
    string courseName;
    int credit;
    int enrollmentLimit;

    // 사용자로부터 강의 정보 입력 받음
    cout << "과목 코드를 입력하세요: ";
    cin >> courseId;

    // 중복 체크: 입력한 과목 코드가 이미 존재하는지 확인
    string duplicateCheckQuery = "SELECT * FROM course_table WHERE course_id='" + courseId + "'";

    // 중복 체크 쿼리 수행
    if (mysql_query(conn, duplicateCheckQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(conn);

        // 결과가 존재하는 경우 (중복된 과목 코드가 존재하는 경우)
        if (result && mysql_num_rows(result) > 0) {
            cout << "이미 존재하는 과목 코드입니다. 다른 과목 코드를 입력해주세요." << endl;
            return;  // 중복된 과목 코드면 함수 종료
        }
    }
    else {
        cout << "중복 체크 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
        return;  // 중복 체크 중 오류 발생하면 함수 종료
    }

    // 나머지 강의 정보 입력 받음
    cout << "과목명을 입력하세요: ";
    cin >> courseName;

    cout << "수강 학점을 입력하세요: ";
    cin >> credit;

    cout << "수강 인원 제한을 입력하세요: ";
    cin >> enrollmentLimit;

    // 데이터베이스에 과목 정보 삽입
    string insertQuery = "INSERT INTO course_table (course_id, course_name, credit, enrollment_limit) VALUES ('" + courseId + "', '" + courseName + "', " + to_string(credit) + ", " + to_string(enrollmentLimit) + ")";

    // 강의 정보 데이터베이스에 삽입
    if (mysql_query(conn, insertQuery.c_str()) == 0) {
        cout << "과목 정보가 성공적으로 개설되었습니다." << endl;
    }
    else {
        cout << "과목 정보 개설 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
    }
}

// 강의를 삭제하는 함수
void deleteCourse() {
    string courseId;

    // 모든 강의 목록 출력
    displayAllCourses();

    // 삭제할 강의 코드 입력 받음
    cout << "삭제할 과목 코드를 입력하세요: ";
    cin >> courseId;

    // 해당 강의가 존재하는지 확인하는 쿼리
    string checkCourseQuery = "SELECT * FROM course_table WHERE course_id='" + courseId + "'";

    // 강의 존재 여부 확인
    if (mysql_query(conn, checkCourseQuery.c_str()) == 0) {
        MYSQL_RES* checkCourseResult = mysql_store_result(conn);

        // 강의가 존재하는 경우에만 삭제 진행
        if (checkCourseResult && mysql_num_rows(checkCourseResult) > 0) {
            string deleteQuery = "DELETE FROM course_table WHERE course_id='" + courseId + "'";

            // 강의 삭제 쿼리 수행
            if (mysql_query(conn, deleteQuery.c_str()) == 0) {
                cout << "강의가 성공적으로 삭제되었습니다." << endl;
            }
            else {
                cout << "강의 삭제 중 오류 발생. 오류 메시지: " << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "해당 강의가 존재하지 않습니다. 삭제를 진행할 수 없습니다." << endl;
        }
    }
    else {
        cout << "강의 존재 확인 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
    }
}

// 모든 강의 목록을 출력하는 함수
void displayAllCourses() {
    // 모든 강의 정보를 가져오는 쿼리
    string allCoursesQuery = "SELECT * FROM course_table";

    // 강의 정보 쿼리 수행
    if (mysql_query(conn, allCoursesQuery.c_str()) == 0) {
        MYSQL_RES* allCoursesResult = mysql_store_result(conn);

        // 결과가 있는 경우
        if (allCoursesResult) {
            int numCourses = mysql_num_rows(allCoursesResult);

            // 존재하는 강의가 있는 경우
            if (numCourses > 0) {
                cout << "[ 현재 개설된 모든 강의 목록 ]" << endl;

                // 모든 강의 정보 출력
                while (MYSQL_ROW courseRow = mysql_fetch_row(allCoursesResult)) {
                    cout << "강의 코드: " << courseRow[0] << ", 강의 이름: " << courseRow[1] << ", 학점: " << courseRow[2] << ", 등록 인원: " << courseRow[3] << endl;
                }
            }
            else {
                cout << "현재 개설된 강의가 없습니다." << endl;
            }
        }
        else {
            cout << "강의 목록을 조회하는 데 실패했습니다. 오류: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "강의 목록을 조회하는 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
    }
}

// 강의에 등록하는 함수
void enrollInCourse(const string& studentId, const string& courseId) {
    string enroll_query = "INSERT INTO enroll_table (student_id, course_id) VALUES ('" + studentId + "', '" + courseId + "')";

    // 강의에 등록하는 쿼리 수행
    if (mysql_query(conn, enroll_query.c_str()) != 0) {
        cout << "강의에 등록하는 데 실패했습니다. 오류: " << mysql_error(conn) << endl;
    }
}

// 학생이 강의에 등록하는 함수
void enrollCourse(const string& studentId, const string& courseId) {
    // 등록 가능한지 확인 후 등록 수행
    if (canEnroll(courseId)) {
        enrollInCourse(studentId, courseId);
        cout << "등록 성공." << endl;
    }
    else {
        cout << "등록 실패. 강의가 가득 찼거나 오류가 발생했습니다." << endl;
    }
}

// 강의에 등록할 수 있는지 확인하는 함수
bool canEnroll(const string& courseId) {
    // 등록 가능한 수용 인원 가져오기
    int enrollmentLimit = getEnrollmentLimit(courseId);

    // 수용 인원을 가져오지 못한 경우
    if (enrollmentLimit != -1) {
        // 현재 등록된 학생 수 조회하는 쿼리
        string status_query = "SELECT COUNT(*) FROM enroll_table WHERE course_id='" + courseId + "'";

        // 등록 상태 쿼리 수행
        if (mysql_query(conn, status_query.c_str()) == 0) {
            MYSQL_RES* result = mysql_store_result(conn);

            // 결과가 있는 경우
            if (result) {
                MYSQL_ROW row = mysql_fetch_row(result);
                int currentEnrollment = atoi(row[0]);

                // 등록 가능 여부 반환 (수강 인원이 수용 인원보다 작은 경우에만 가능)
                return currentEnrollment < enrollmentLimit;
            }
            else {
                cout << "등록 상태를 검색하는 데 실패했습니다. 오류: " << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "등록 상태를 질의하는 중 오류가 발생했습니다. 오류: " << mysql_error(conn) << endl;
        }
    }

    return false;  // 오류를 나타냄
}


int main() {
    int user = -1;
    int status = -1;

    // DB 설정
    struct DB_Connection {
        const char* server = "외부 IP",
            * user = "sejoon",
            * pw = "dkstpwns1!",
            * db = "cpp_project";
        unsigned int port = 33060;  // 포트 변경
    } DBCon;

    // MySQL 연결 객체 초기화
    conn = mysql_init(NULL);

    if (conn == NULL) {
        cout << "mysql_init() failed" << endl;
        return 0;
    }

    // MySQL 라이브러리 초기화
    if (mysql_library_init(0, NULL, NULL) != 0) {
        cout << "mysql_library_init() failed" << endl;
        return 0;
    }

    // 문자셋 설정
    if (mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8") != 0) {
        cout << "mysql_options(MYSQL_SET_CHARSET_NAME) failed" << endl;
        return 0;
    }

    // MySQL 데이터베이스에 연결
    if (mysql_real_connect(conn, DBCon.server, DBCon.user, DBCon.pw, DBCon.db, DBCon.port, NULL, 0)) {
        cout << "DB에 연결되었습니다." << endl;
        cout << "[ 수강신청 프로그램 ]" << endl;
    }
    else {
        cout << "DB에 연결되지 않았습니다. 오류 메시지: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 0;
    }

    // 메인 루프
    while (status == -1) {
        // 메뉴를 표시하고 사용자 입력을 받음
        user = main_menu();

        switch (user) {
        case 1:
            // 회원 가입 함수 호출
            signup();
            break;
        case 2:
            // 학생 로그인 함수 호출
            studentLogin();
            break;
        case 3:
            // 관리자 로그인 함수 호출
            adminLogin();
            break;
        case 0:
            // 프로그램 종료
            status = -999;
            mysql_close(conn);
            break;
        default:
            cout << "잘못된 입력입니다. 다시 입력해주세요." << endl;
            main_menu();  // 잘못된 입력 시 메뉴 다시 표시
            break;
        }
    }

    return 0;
}
