char* cal_plus(char *A, char *B) {  // 덧셈
    int i, sum, oneA, oneB, carry = 0, length, longer_length;
    char *a, *b;
    a=A; b=B;
    char *return_result, doPlus, *temp, *finalResult;
    if(strlen(a) >= strlen(b)) {
        longer_length = strlen(a) + 1;
    } else {
        longer_length = strlen(b) + 1;
    }
    temp = (char*)calloc(sizeof(char), longer_length);
    return_result = (char*)calloc(sizeof(char), longer_length);
    finalResult = (char*)calloc(sizeof(char), longer_length);
    if (a[0] == '-') {
        if (b[0] == '-') {  // -1 + -3
            a++;  //pointer이동해서 X값에서 음수 부호 빼주기
            b++;
            return_result = cal_plus(a, b);
            finalResult = (char*)calloc(sizeof(char), strlen(return_result) + 1);
            strcat(finalResult, "-");
            strcat(finalResult, return_result);
            free(return_result);
            return finalResult;
        }
        else {  // -1 + 3
            a++;
            return cal_minus(b, a); //X에서 -만 빼기
        }
    }
    else if (b[0] == '-') { // 1 + (-3)
        b = b + 1;
        return_result = cal_minus(b, a);
        finalResult = malloc(strlen(return_result) + 1);
        for (i = 0; i <= strlen(return_result); i++) {
            if (*return_result == '0')
                return_result++;
            else
                break;
        }
        strcat(finalResult, "-");
        strcat(finalResult, result);
        free(result);
        return finalResult;
    } // end if(X[0] == '-')
    // -----------------------------------------------------------
    if(find_dot(a) > find_dot(b))
        b = fill_zero_int(a, b);
    else if(find_dot(b) > find_dot(a))
        a = fill_zero_int(b, a);
    if(strlen(a) - find_dot(a) > strlen(b) - find_dot(b))
        b = fill_zero_dec(a, b);
    else if(strlen(b) - find_dot(b) > strlen(a) - find_dot(a))
        a = fill_zero_dec(b, a);
    
    length = strlen(a);
    for (i = 0; i < length - 1; i++, a++, b++) continue;  // A와 B의 포인터 위치를 맨 끝으로 이동하여 맨 뒤부터 계산하도록 함
    for (i = length - 1; i >= 0; i--, a--, b--) {  // 맨 뒤부터 앞으로 한칸씩 수행
        if (*a == '.' || *b == '.') {  // 소수점을 만나게 되면 점을 찍고 다음 반복 수행
            return_result[i] = '.';
            continue;
        }
        oneA = *a - 48;  // ASCII 값 중 0~9로 위치하도록 보정
        oneB = *b - 48;
        sum = oneA + oneB;  // 덧셈 수행
        if (carry) {  // 지난 덧셈에서 carry가 있을 경우 sum에 carry를 더함
            sum += carry;
            carry = 0;  // carry 사용 후 0으로 초기화
        }
        doPlus = (sum % 10) + 48;  // ASCII 0~9값 설정
        if (sum >= 10) carry++;  // 만약 합이 10보다 클 경우 carry++
        return_result[i] = doPlus;  // 현재 위치에 덧셈 결과 넣음
    }
    if (carry) {  // 맨 앞까지 덧셈을 끝낸 후 carry가 있다면 결과값 맨 앞에 1을 추가
        strcat(temp, "1");
        strcat(temp, return_result);
        return_result = temp;
    }
    return return_result;
}  // end cal_plus

int abs_compare(char* X, char* Y) {
    int length_A = find_dot(X);
    int length_B = find_dot(Y);
    char *A = X, *B = Y;
    if (A[0] == '-') A++;
    if (B[0] == '-') B++;
    if (length_A > length_B)
        return 1;
    else if (length_A == length_B) {
        while (*A != '\0') {
            if (*A > *B)
                return 1;
            else {
                A++; B++;
            }
        }
    }
    return 0;
}  // end abs_compare()

char* cal_minus(char *A, char *B) {  // 뺄셈
    int i, sum, oneA, oneB, carry = 0, exception_minus = 0, A_bigger_abs = 0, length, longer_length;
    char *a, *b;  // A와 B를 바꿀 부분  <<----- B > A 라서 뺄셈시 음수가 될 때
    a = A; b = B;  // X,Y로 받아 A,B로 이름 바꿔서 사용
    char *return_result, doMinus, *temp, *finalResult;
    if(strlen(a) >= strlen(b)) {
        longer_length = strlen(a) + 1;
    } else {
        longer_length = strlen(b) + 1;
    }
    temp = malloc(sizeof(char) * longer_length);
    return_result = malloc(sizeof(char) * longer_length);
    finalResult = malloc(sizeof(char) * longer_length);
    A_bigger_abs = abs_compare(a, b);

    if (a[0] != '-' && b[0] != '-' && abs_compare(b, a)) {
        B = a;
        A = b;
        exception_minus = 1;
    }
    if (a[0] == '-') {
        if (b[0] == '-') {
            if (A_bigger_abs) { // if abs A > abs B => -1 = - (19 - 18)
                a++;
                b++;
                return_result = cal_minus(a, b);
                strcat(finalResult, "-");
                strcat(finalResult, return_result);
                return finalResult;
            } else {  // not A_bigger_abs
                a++;
                b++;
                return_result = cal_minus(b, a);
                return return_result;
            }
        }  // end if(b[0] == '-')
        else { // -1 - 8
            a++;  //X에서 -만 빼기
            return_result = cal_plus(a, b);
            strcat(finalResult, "-");
            strcat(finalResult, return_result);
            return finalResult;
        }
    }  // end if(a[0] == '-')
    else if (b[0] == '-') { // 1 + (-3)
        b++;
        return_result = cal_plus(a, b);
        return return_result;
    }
    //------------------------------------------------------
    if (!A_bigger_abs) {
        return_result = cal_minus(b, a);
        strcat(temp, "-");
        strcat(temp, return_result);
        return temp;
    }
    if(find_dot(a) > find_dot(b))
        b = fill_zero_int(a, b);
    else if(find_dot(b) > find_dot(a))
        a = fill_zero_int(b, a);
    if(strlen(a) - find_dot(a) > strlen(b) - find_dot(b))
        b = fill_zero_dec(a, b);
    else if(strlen(b) - find_dot(b) > strlen(a) - find_dot(a))
        a = fill_zero_dec(b, a);
    length = strlen(a);
    for (i = 0; i < length - 1; i++, a++, b++) ;  // A와 B의 포인터 위치를 맨 끝으로 이동하여 맨 뒤부터 계산하도록 함
    for (i = length - 1; i >= 0; i--, a--, b--) {  // 계산부
        if (*a == '.' || *b == '.') {  // 소수점을 만나게 되면 점을 찍고 다음 반복 수행
            return_result[i] = '.';
            continue;
        }
        oneA = *a - 48;  // ASCII 값 중 0~9로 위치하도록 보정
        oneB = *b - 48;
        sum = oneA - oneB;  // 뺄셈 수행
        if (carry) {  // 직전 반복 뺼셈에서 carry 발생했을 경우 -1
            sum--;
            carry = 0;  // carry 사용 후 0으로 초기화
        }
        if (sum < 0 && i == 0) {  // 맨 앞자리 뺄셈인데 음수일 경우
            sum *= -1;  // 양수화
            carry++;  // 캐리 ++
        }
        else if (sum < 0) {  // 뺄셈이 음수일 경우
            sum += 10;  // 10을 더하고 캐리 ++
            carry++;
        }
        doMinus = (sum % 10) + 48;  // ASCII 0~9값 설정
        return_result[i] = doMinus;
    }
    return_result[strlen(return_result)] = '\0';
    return return_result;
}  // end cal_minus

char* fill_zero_int(char* A, char* B) {  // 덧셈, 뺄셈시 정수부 자리수가 차이나는 만큼 0으로 채움
    int int_A = find_dot(A);  // A의 정수부 길이
    int int_B = find_dot(B);  // B의 정수부 길이
    int difference_int = int_A > int_B ? int_A - int_B : int_B - int_A;
    char *temp_A = A;  // A를 temp_A에 복사
    char *temp_B = B;  // B를 temp_B에 복사
//    char *temp = (char*)calloc((strlen(A) + difference_int) , sizeof(char));  // 두 수의 자릿수가 차이나는 만큼 0으로 채울 공간
    char *temp = (char*)calloc(sizeof(char), strlen(A));
    char *temp2 = (char*)calloc(sizeof(char), strlen(A));

    //    difference_int = int_A - int_B;
    if (int_A > int_B) {  // 정수부가 A가 B보다 긴 경우
        for (; difference_int > 0; difference_int--)  // 차이나는 만큼 0으로 채움
            strcat(temp, "0");
        //        strcat(tempInt, temp_B);
        //        return tempInt;
    }
    strcat(temp2, temp);
    strcat(temp2, temp_B);
    return temp2;
}  // end plus
