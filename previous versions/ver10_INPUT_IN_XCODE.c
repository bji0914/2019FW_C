  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define print printf
#define STDOUT 1
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
const int CALLOCSIZE = 200;  // 동적 할당시 임시로 사용할 사이즈. 코드 완성 후엔 각 선언마다 적절한 lenth로 계산해서 사용할 것
int find_dot(char*);
char* fill_zero_int(char*, char*);
char* fill_zero_dec(char*, char*);
char* cal_plus(char*, char*);
char* cal_minus(char*, char*);
int abs_compare(char*, char*);
char* minus_exception(char*, char*);
char *result;
char* find_first_negative(char*);
char* cal_multiply(char*, char*);
int cal_decimal_length(char*, char*);
char* add_dot(char* ,int);
char* mul_add(char*, char*);
char* remove_dot(char*);
char* remove_zero(char*);
int A_negative = 0;
int B_negtive = 0;
int first_negative = 0;  // 첫 숫자가 음수일 경우 1, default 0
//int A_bigger_abs = 0;  // 첫 숫자 A가 음수이며 덧셈할 B보다 값이 더 클 경우 1, default 0
int multiply_decimal = 0;  // 소수가 있는 수의 곱셈시 1, default 0
int top, size;
int *stack;
void init_stack() { top = -1; }
int push(int t);
int pop();
int get_top() { return (top < 0) ? -1 : stack[top]; }
int is_empty() { return (top < 0); }
int is_operator(int k) { return (k == '+' || k == '-' || k == '*' || k == '/'); }
int is_legal(char *s); // 후위표기법 수식이 적접한가 체크
int precedence(int op); // 연산자의 우선순위를 수치로 변환해준다.
int postfix(char *dst, char*src);// 중위표기법을 후위표기법으로 변환
void calc(char *p, int len); //후위표기법 계산
// end definition
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
enum {OFF, ON};
const int COMPILE_AT_XCODE = ON;  // 0 = gcc compile, 1 = xcode compile
const char* INPUT_IN_XCODE = "222222222222.22-1111111.11";
const int PRINT_ON = ON;  // stat=ON : print A, B, answer in calc
void print_in_calc(char*, char*, char*);
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
int main(int argc, char *argv[]) {
    char *input;
    char *post_fix;
    char *filename;
    int fp, readn;
    int answer;

    // 이 프로그램이 실행되기 위해서는 파일이름을 인자로 받는다.
    // 만약 인자가 부족하다면, 프로그램의 사용방법을 출력하고 종료한다.
    if(!COMPILE_AT_XCODE) {
        if (argc != 2)
        {
            printf("Usage : %s [file]\n", argv[0]);
            return 1;
        }

        // filename 이 argv[1] 을 가리킨다.
        // argv[1] 을 그대로 사용해도 되겠지만, 코드의 가독성을 위해서
        // 다른 변수이름을 사용하도록 했다.
        filename = argv[1];
        fp = open(filename, O_RDONLY); // 파일을 읽기전용 모드로 연다.
        if (fp < 0)
        {
            perror("file open err\n");
            return 1;
        }

        size = lseek(fp, 0, SEEK_END);
    }  // end if(!COMPILE_AT_XCODE)
    
    if(COMPILE_AT_XCODE) size = strlen(INPUT_IN_XCODE);
    
    input = malloc(size + 1); //파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
    post_fix = malloc(size + 1);
    stack = malloc(sizeof(int) * size);
    memset(input, 0, size + 1); //파일 크기 + 1바이트만큼 메모리를 0으로 초기화
    memset(post_fix, 0, size + 1);

    if(!COMPILE_AT_XCODE) {
        lseek(fp, 0, 0);

    // read 함수를 이용해서, 파일지시자로 부터 데이터를 읽어들인다.
    // read 함수는 읽어들인 데이터의 크기를 리턴한다.
    // 더이상 읽을 데이터가 없다면 0을 리턴하니, 그때 while 루프를 빠져나오면 된다.
        while ((readn = read(fp, input, size)) > 0)
            write(STDOUT, input, readn); // 읽어들인 데이터의 크기만큼 화면에 출력.

        if (!is_legal(input)) {
            print("\n!! Unavailable input !!\n");
            return 1;
        }
    }  // end if(!COMPILE_AT_XCODE)
    
    if(COMPILE_AT_XCODE) {
        input = INPUT_IN_XCODE;
        printf("in XCode input : %s", input);
    }  // end if(COMPILE_AT_XCODE)
    //printf("%s\nsize: %d\n", input, size);
    int count = postfix(post_fix, input); // 최대 글자수
    printf("\nPostfix : %s\n", post_fix);

    calc(post_fix, count);
    if(!COMPILE_AT_XCODE) {
        close(fp);
        free(input);
        free(post_fix);
        free(stack);
    }  // end if(!COMPILE_AT_XCODE)
}  // end main()

char* cal_multiply(char *a, char *b) {  // 곱셈
   int i , j, sum,k,strlenA,strlenB,dot = 0;
   int count = 0;
   int cnt = 0;
   int nflag = 0;
   char one ;
   char *result = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *ptr;
   char *answer = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *temp = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *temp2 = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *temp3 = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *temp4 = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *negative = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *c = malloc(sizeof(char)*strlen(a)*strlen(b));
   char *d = malloc(sizeof(char)*strlen(a)*strlen(b));
   memset(answer,'X',sizeof(char)*strlen(a)*strlen(b));
   memset(temp,'X',sizeof(char)*strlen(a)*strlen(b));
   memset(temp2,'X',sizeof(char)*strlen(a)*strlen(b));
   memset(temp3,'X',sizeof(char)*strlen(a)*strlen(b));
   memset(temp4,'X',sizeof(char)*strlen(a)*strlen(b));
   memset(negative,'X',sizeof(char)*strlen(a)*strlen(b));

   result = strchr(a,'.');

   if(result != NULL){

        c = remove_dot(a);
        strlenA = strlen(a)-1;
        dot = strlen(a)-find_dot(a)-1 +dot;
   }
   else{
        c = a;
        strlenA = strlen(a);
        
   }
   cnt = 0;
   
   
   result = strchr(b,'.');

   if(result != NULL){
        d = remove_dot(b);
        strlenB = strlen(b)-1;
        dot = strlen(b)-find_dot(b)-1 +dot;
   }
   else{
        d = b;
        strlenB = strlen(b);
        
   }
   cnt = 0;

   if((*c=='-' && *d=='-')||(*c!='-' && *d!='-')){
        if(*c=='-' && *d=='-'){
            strncpy(temp3,c+1,strlenA);
            c=temp3;
            strlenA--;

            strncpy(temp4,d+1,strlenB);
            d=temp4;
            strlenB--;
        }
   }
   else{

        nflag = 1;
        if(*c=='-'){
            strncpy(temp3,c+1,strlenA);
            c=temp3;
            strlenA--;
        }
        else if(*d=='-'){
            strncpy(temp3,d+1,strlenB);
            d=temp3;
            strlenB--;
        }
   }
   for(i = 0; i < strlenA-1; i++, c++) continue;
   for(j = 0; j < strlenB-1; j++, d++) continue;
   sum = 0;
   for (i = strlenB ; i > 0 ; i--, d--){
      answer[strlenA+cnt+1] = '\0';  
      for (j = strlenA+cnt; j > strlenA; j--){
         answer[j] = '0';
      }
      for (j = strlenA; j > 0; j--,c--){
         int oneA = *c-48;
         int oneB = *d-48;

         sum = oneA*oneB + sum;
         if(sum>9){
            one = (sum%10)+48;
            sum = sum/10;
         }
         else{
            one = sum+48;
            sum = 0;
         }
         answer[j] = one;
      }
      if(sum>0){
         answer[j] = sum+48;
      }
      if(answer[0] == 'X'){
        strncpy(temp,answer+1,strlenA+cnt+1);
        answer = temp;
      }
      if(count==0){
        strcpy(temp2,answer);
      }
      else{
        strcpy(temp2,mul_add(temp2,answer));
      }
      cnt++;
      count++;
      sum = 0;
      memset(answer,'X',sizeof(char)*strlen(a)*strlen(b));
      for(k = 0; k < strlenA; k++, c++) continue;
  }
  if(dot==0){
    strcpy(temp2,remove_zero(temp2));
    if(nflag==1){
         negative[0] = '-';
        negative[1] = '\0';
        strcat(negative, temp2); 
        strcpy(temp2,negative);
    }
    return temp2;
  }
  
  else{
    strcpy(temp2,add_dot(temp2,dot));
    printf("%s\n",temp2 );
    strcpy(temp2,remove_zero(temp2));
    if(nflag==1){
      negative[0] = '-';
      negative[1] = '\0';
      strcat(negative, temp2); 
      strcpy(temp2,negative);
    }
    return temp2;
  }
  free(result);
  free(answer);
  free(temp);
  free(temp2);
  free(temp3);
  free(c);
  free(d);
}  // end cal_multiply()
char* mul_add(char *a, char *b){

  if(strlen(a)>strlen(b)){
    int i = 0;
    int sum = 0;
    int one = 0;
    int len = strlen(a)+1;
    int lenA = strlen(a);
    int lenB = strlen(b);
    char *temp = malloc(sizeof(char)*lenA*lenB);
    memset(temp,'X',sizeof(char)*len);
    temp[len] = '\0';
    len--;

    for (i = 0; i < lenA-1; ++i,a++) continue;
    for (i = 0; i < lenB-1; ++i,b++) continue;

    for (i = lenB; i > 0; i--,b--,a--,len--){
      int oneA = *a-48;
      int oneB = *b-48;
      sum = oneA+oneB + sum;
      if(sum>9){
        one = (sum%10)+48;
        sum = sum/10;
      }
      else{
        one = sum+48;
        sum = 0;
      }
      temp[len] = one;
    }
    
    while(len>0){
      int oneA = *a-48;
      if(oneA+sum>9){
        temp[len] = ((oneA+sum)%10)+48;
        sum = 1;
      }
      else if(sum>0){
        temp[len] = (oneA+sum)+48;
        sum = 0;
      }
      else{
        temp[len] = oneA+48;
      }
      len--;
      a--;
    }
    if(sum>0) temp[0] = sum+48;
    else{
      strncpy(temp,temp+1,lenA);
      temp[lenA] = '\0';
    }
    return temp;
    free(temp);
  }
  else if(strlen(a)<strlen(b)){
    int i = 0;
    int sum = 0;
    int one = 0;
    int len = strlen(b)+1;
    int lenA = strlen(a);
    int lenB = strlen(b);
    char *temp = malloc(sizeof(char)*lenA*lenB);
    memset(temp,'X',sizeof(char)*lenA*lenB);
    temp[len] = '\0';
    len--;

    for (i = 0; i < lenA-1; ++i,a++) continue;
    for (i = 0; i < lenB-1; ++i,b++) continue;

    for (i = lenA; i > 0; i--,b--,a--,len--){
      int oneA = *a-48;
      int oneB = *b-48;
      sum = oneA+oneB + sum;
      if(sum>9){
        one = (sum%10)+48;
        sum = sum/10;
      }
      else{
        one = sum+48;
        sum = 0;
      }
      temp[len] = one;
    }
    
    while(len>0){
      int oneB = *b-48;
      if(oneB+sum>9){
        temp[len] = ((oneB+sum)%10)+48;
        sum = 1;
      }
      else if(sum>0){
        temp[len] = (oneB+sum)+48;
        sum = 0;
      }
      else{
        temp[len] = oneB+48;
      }
      len--;
      b--;
    }
    if(sum>0) temp[0] = sum+48;
    else{
      strncpy(temp,temp+1,lenB);
      temp[lenB] = '\0';
    }
    return temp;
    free(temp);
  }
  else{
    int i = 0;
    int sum = 0;
    int one = 0;
    int len = strlen(a)+1;
    int lenA = strlen(a);
    int lenB = strlen(b);

    char *temp = malloc(sizeof(char)*lenA*lenB);
    memset(temp,'X',sizeof(char)*len);
    temp[len] = '\0';
    len--;

    for (i = 0; i < lenA-1; ++i,a++) continue;
    for (i = 0; i < lenB-1; ++i,b++) continue;

    for (i = lenB; i > 0; i--,b--,a--,len--){
      int oneA = *a-48;
      int oneB = *b-48;
      sum = oneA+oneB + sum;
      if(sum>9){
        one = (sum%10)+48;
        sum = sum/10;
      }
      else{
        one = sum+48;
        sum = 0;
      }
      temp[len] = one;
    }
    if(sum>0){
      temp[0] = sum+48;
    }
    else{
      strncpy(temp,temp+1,lenA);
      temp[lenA] = '\0';
    }
    return temp;
    free(temp);
  }
}

char* remove_dot(char *str){
    int cnt = 0,len = strlen(str);
    char *ptr;
    char *a = malloc(sizeof(char)*len);
    char *b = malloc(sizeof(char)*len);
    char *c = malloc(sizeof(char)*len);
    strcpy(a,str);
    ptr = strtok(a, ".");
    while(ptr != NULL ){
        if(cnt==0) b = ptr;
        else c = ptr;
        ptr = strtok(NULL, ".");
        cnt++;
    }
    strcat(b,c);
    return b;
}
char* add_dot(char *str,int dot){
    int i;
    int len2 = strlen(str);
    char *answer = malloc(sizeof(char)*len2);
    char *temp = malloc(sizeof(char)*len2);
    memset(answer,'X',sizeof(char)*len2);
    memset(temp,'X',sizeof(char)*len2);
    strcpy(answer,str);
    int len = strlen(answer);
    temp[len] = '\0';
    for (i = 0; i <= len+1; ++i,answer++){
        if(i+dot==len){
          temp[i] = '.';
          answer--;
        }
        else temp[i] = *answer;
    }
    return temp;
    free(answer);
    free(temp);
}

char* remove_zero(char* str){

  int count = 0,dotlen = 0,dotcnt = 0,i = 0;
  int len = strlen(str);
  char *temp = malloc(sizeof(char)*len*2);
  char *temp2 = malloc(sizeof(char)*len*2);
  char *result = malloc(sizeof(char)*len*2);
  memset(temp,'X',sizeof(char)*len*2);
  memset(temp2,'X',sizeof(char)*len*2);
  memset(result,'X',sizeof(char)*len*2);
  strcpy(temp,str);
  
  if(str[0]=='0'){

    while(1){
      if(*temp=='.'){
        //count--;
        break;
      }
      else if(*temp!='0') {
        break;
      }
      else{
        count++;
        temp++;
      }
    }
    strncpy(str,str+count-1,len);
  }
  strcpy(temp2,str);
  dotlen = strlen(temp2);
  result = strchr(str,'.');
  if(result != NULL){
    if(temp2[dotlen-1]=='0'){

      for (i = 0; i < dotlen-1; ++i,temp2++) continue;
      while(1){
        if(*temp2=='.'||*temp2!='0'){
          if(*temp2=='.') dotcnt++;
          break;
        }
        else{
          *temp2--;
          dotcnt++;
        }
      }
      str[dotlen-dotcnt] = '\0';
    }
  }
  return str;
}
int cal_decimal_length(char* X, char* Y) {  // 곱셈시 각 수의 소수부 길이를 계산하여 합 return
    int len = 0, a, b;
    a = strlen(X) - find_dot(X);
    b = strlen(Y) - find_dot(Y);
    if (a > 0)
        len += a - 1;
    if (b > 0)
        len += b - 1;
    if (len > 0)
        multiply_decimal = 1;
    return len;
}  // end cal_decimal_length()
char* cal_plus(char *X, char*Y) {  // 덧셈
//    if(first_negative) return minus_exception(X, Y);
    int i, sum, oneA, oneB, carry = 0;
    char *A = X, *B = Y;  // X,Y로 받아 A,B로 이름 바꿔서 사용
    char *return_result, doPlus, *temp, *finalResult;
    A = fill_zero_int(B, A);
    B = fill_zero_int(A, B);
    A = fill_zero_dec(B, A);
    B = fill_zero_dec(A, B);
    int length_A = strlen(A);
    temp = (char*)calloc(sizeof(char), CALLOCSIZE);
    return_result = (char*)calloc(sizeof(char), strlen(A));
    finalResult = (char*)calloc(sizeof(char), strlen(A) + 1);
    //    printf("X : %s , Y : %s \n", X, Y);
    if (X[0] == '-') {
        if (Y[0] == '-') {// -1 + -3
            printf("X : %s , Y : %s \n", X, Y);
            X = X + 1; //pointer이동해서 X값에서 음수 부호 빼주기
            Y = Y + 1;
            X = fill_zero_int(Y, X);
            Y = fill_zero_int(X, Y);
            X = fill_zero_dec(Y, X);
            Y = fill_zero_dec(X, Y);
            printf("X : %s , Y : %s \n", X, Y);
            return_result = cal_plus(X, Y);
            finalResult = (char*)calloc(sizeof(char), strlen(return_result) + 1);
            //            printf("result : %s \n", result);
            for (i = 0; i <= strlen(return_result); i++) {
                if (*return_result == '0')
                    return_result++;
                else
                    break;
            }
            strcat(finalResult, "-");
            strcat(finalResult, return_result);
            free(return_result);
            //            print("final result : %s \n", finalResult);
            return finalResult;
        }
        else { // -123 + 15 15-|-123| -> 15 - 123 = - ( 123 - 15 )
//            printf("X : %s , Y : %s \n", X, Y);
            X = X + 1;
            //            X = fill_zero_int(Y, X);
            //            Y = fill_zero_int(X, Y);
            //            X = fill_zero_dic(Y, X);
            //            Y = fill_zero_dic(X, Y);
            return cal_minus(Y, X); //X에서 -만 빼기
        }
    }
    else if (Y[0] == '-') { // 1 + (-3)
        Y = Y + 1;
        X = fill_zero_int(Y, X);
        Y = fill_zero_int(X, Y);
        X = fill_zero_dec(Y, X);
        Y = fill_zero_dec(X, Y);
        return_result = cal_minus(Y, X);
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
        //        print("final result : %s \n", finalResult);
        return finalResult;
    } // end if(X[0] == '-')
//    print("X : %s , Y : %s \n", X, Y);
    // -----------------------------------------------------------

    for (i = 0; i < length_A - 1; i++, A++, B++) continue;  // A와 B의 포인터 위치를 맨 끝으로 이동하여 맨 뒤부터 계산하도록 함
    for (i = length_A - 1; i >= 0; i--, A--, B--) {  // 맨 뒤부터 앞으로 한칸씩 수행
        if (*A == '.' || *B == '.') {  // 소수점을 만나게 되면 점을 찍고 다음 반복 수행
            return_result[i] = '.';
            continue;
        }
        oneA = *A - 48;  // ASCII 값 중 0~9로 위치하도록 보정
        oneB = *B - 48;
        sum = oneA + oneB;  // 덧셈 수행
        if (carry) {  // 지난 덧셈에서 carry가 있을 경우 sum에 carry를 더함
            sum += carry;
            carry = 0;  // carry 사용 후 0으로 초기화
        }
        doPlus = (sum % 10) + 48;  // ASCII 0~9값 설정
        if (sum >= 10) carry++;  // 만약 합이 10보다 클 경우 carry++
        return_result[i] = doPlus;  // 현재 위치에 덧셈 결과 넣음
//        printf("i : %d, return_result[i] : %c\n", i, return_result[i]);
    }
    //    printf("len : %d\n", strlen(return_result));
    //    printf("final return_result : %s\n", return_result);
    if (carry) {  // 맨 앞까지 덧셈을 끝낸 후 carry가 있다면 결과값 맨 앞에 1을 추가
        strcat(temp, "1");
        strcat(temp, return_result);
        return_result = temp;
    }
    return return_result;
}  // end cal_plus
char* find_first_negative(char* X) {  // 첫 숫자가 음수일 경우 global first_negative를 켜주고 음수를 뗀 숫자 return
    if (*X == '-') {
        X++;
        first_negative = 1;
    }
    return X;
}  // end find_first_negative()
int abs_compare(char* X, char* Y) {
    int length_A = find_dot(X);
    int length_B = find_dot(Y);
    //    printf("!!!X : %s , Y : %s \n", X, Y);
    //
    //    if (X[0] == '-') X++;
    //    if (Y[0] == '-') Y++;
    //    printf("!!!!!!!X : %s , Y : %s \n", X, Y);
    char *A = X, *B = Y;
    if (A[0] == '-') A++;
    if (B[0] == '-') B++;
    if (length_A > length_B)
        return 1;
    else if (length_A == length_B) {
        //        printf("!@@@@!A : %c , B : %c \n", *A, *B);
        while (*A != '\0') {
            if (*A > *B)
                return 1;
            else {
                //            printf("!@@@@!A : %c , B : %c \n", *A, *B);
                A++; B++;
                //                printf("!@@######@!A : %c , B : %c \n", *A, *B);
            }
        }
    }
    return 0;
}  // end abs_compare()
char* minus_exception(char *X, char *Y) {
    // -5 + 11  -> (11-5) =  6 -> -A + B = +(B - A)
    // -5 +  2  -> -(5-2) = -3 -> -A + B = -(A - B)  -> ** 예외 처리 할 case **
    // 음수 + 양수 의 경우 음수, 양수간의 절대값 대수 비교가 필요.  abs_compare() returns absolute value compare
    // 계산은 절대값간의 뺄셈으로 이루어지나, 음수가 더 클 경우 최종 결과 음수 / 양수가 더 클 경우 최종 결과 양수.
    char *A = X, *B = Y;
    char *temp_return, *temp;
    int A_bigger_abs = 0;
    A_bigger_abs = abs_compare(X, Y);  // X의 절대값이 Y보다 클 경우 A_bigger_abs = 1
    first_negative = 0;
    temp_return = (char*)calloc(sizeof(char), CALLOCSIZE);
    if (A_bigger_abs) {  // A<0 && abs(A) > abs(B)
        temp = (char*)calloc(sizeof(char), CALLOCSIZE);
        temp_return = cal_minus(A, B);
        strcat(temp, "-");  // 마이너스 부호 붙임
        strcat(temp, temp_return);
        temp_return = temp;
    }
    else  // A<0 && abs(B) > abs(A)
        temp_return = cal_minus(B, A);
    // end if
    A_bigger_abs = 0;
    return temp_return;
}  // end minus_exception
char* cal_minus(char *a, char*b) {  // 뺄셈
    int i, sum, oneA, oneB, carry = 0, exception_minus = 0;
    char *A, *B;  // A와 B를 바꿀 부분  <<----- B > A 라서 뺄셈시 음수가 될 때
    A = a; B = b;  // X,Y로 받아 A,B로 이름 바꿔서 사용
    char *return_result, doMinus, *temp, *finalResult;
    int length_A = strlen(A);

    temp = malloc(sizeof(char) * 10000);
    return_result = malloc(sizeof(char) * 1000);
    finalResult = malloc(sizeof(char) * 10000);

    int A_bigger_abs = 0;
    A_bigger_abs = abs_compare(a, b);

    if (a[0] != '-' && b[0] != '-' && abs_compare(b, a)) {
        B = a;
        A = b;
        exception_minus = 1;

    }
    //    if(A_bigger_abs) print("!!!!!!!!!");
    if (a[0] == '-') {

        if (b[0] == '-') {// -1 - -3 -> -1 + 3
            // -3 - -1  = -2  '-'
//                X = X+1; //pointer이동해서 X값에서 음수 부호 빼주기
//                Y = Y+1;
//                X = fill_zero_int(Y, X);
//                Y = fill_zero_int(X, Y);
//                X = fill_zero_dic(Y, X);
//                Y = fill_zero_dic(X, Y);
    //            printf("X : %s , Y : %s \n", X, Y);
            if (A_bigger_abs) { // if abs A > abs B => -1 = - (19 - 18)
                a++;
                b++;
                result = cal_minus(a, b);
                for (i = 0; i <= strlen(result); i++) {
                    if (*result == '0')
                        result++;
                    else
                        break;
                }
                strcat(finalResult, "-");
                strcat(finalResult, result);
                return finalResult;
            }
            //            X = -18, Y = -19  if abs B > abs A => 1 = (19 - 18)
            a++;
            b++;
            result = cal_minus(b, a);
            //                finalResult = (char*)calloc(sizeof(char) ,  strlen(result) + 1);
                //            printf("result : %s \n", result);
            //                strcat(finalResult, "-");
            //                strcat(finalResult, result);
            //                free(result);
                //            print("final result : %s \n", finalResult);
            return result;
        }
        else { // -1 - 8
            a = a + 1;
            //                printf("X : %s , Y : %s \n", X, Y);
            //                X = fill_zero_int(Y, X);
            //                Y = fill_zero_int(X, Y);
            //                X = fill_zero_dic(Y, X);
            //                Y = fill_zero_dic(X, Y);
            //                printf("X : %s , Y : %s \n", X, Y);
            result = cal_plus(a, b); //X에서 -만 빼기
            for (i = 0; i <= strlen(result); i++) {
                if (*result == '0')
                    result++;
                else
                    break;
            }
            //                printf("result : %s \n", result);
            strcat(finalResult, "-");
            strcat(finalResult, result);
            return finalResult;
        }
    }
    else if (b[0] == '-') { // 1 + (-3)
        b = b + 1;
        //            X = fill_zero_int(Y, X);
        //            Y = fill_zero_int(X, Y);
        //            X = fill_zero_dic(Y, X);
        //            Y = fill_zero_dic(X, Y);
        char *result = cal_plus(a, b);
        for (i = 0; i <= strlen(result); i++) {
            if (*result == '0')
                result++;
            else
                break;
        }
        //            char *finalResult = malloc(strlen(result) + 1);
        //            strcat(finalResult, "-");
        //            strcat(finalResult, result);
        //            free(result);
            //        print("final result : %s \n", finalResult);
        return result;
    }
    //------------------------------------------------------
    if (!A_bigger_abs) {
        return cal_minus(b, a);
    }
    A = fill_zero_int(B, A);
    B = fill_zero_int(A, B);
    A = fill_zero_dec(B, A);
    B = fill_zero_dec(A, B);

    //    printf("A : %s , B : %s , strlen(A) : %d \n", A, B,strlen(A)-1);
    length_A = strlen(A);
    for (i = 0; i < length_A - 1; i++, A++, B++);  // A와 B의 포인터 위치를 맨 끝으로 이동하여 맨 뒤부터 계산하도록 함
//    printf("X : %s , Y : %s \n", X, Y);

//    printf("A : %s , B : %s \n", A, B);

    for (i = length_A - 1; i >= 0; i--, A--, B--) {
        if (*A == '.' || *B == '.') {  // 소수점을 만나게 되면 점을 찍고 다음 반복 수행
            return_result[i] = '.';
            continue;
        }
        oneA = *A - 48;  // ASCII 값 중 0~9로 위치하도록 보정
        oneB = *B - 48;
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
    if (carry) {  // 계산을 끝까지 마치고도 캐리가 있는 경우 = 계산 결과가 음수가 된 경우
        strcat(temp, "-");  // 마이너스 부호 붙임
        strcat(temp, return_result);
        return_result = temp;
    }
    else if (exception_minus) {
        strcat(temp, "-");  // 마이너스 부호 붙임
        strcat(temp, return_result);
        return_result = temp;
    }
    //int llen = strlen(return_result);
    return_result[strlen(return_result)] = '\0';
    //printf("%s \n",return_result);
    return return_result;
}  // end cal_minus
char* fill_zero_int(char* A, char* B) {  // 덧셈, 뺄셈시 정수부 자리수가 차이나는 만큼 0으로 채움
    int int_A = find_dot(A);  // A의 정수부 길이
    int int_B = find_dot(B);  // B의 정수부 길이
    int difference_int = int_A > int_B ? int_A - int_B : int_B - int_A;
    char *temp_A = A;  // A를 temp_A에 복사
    char *temp_B = B;  // B를 temp_B에 복사
//    char *temp = (char*)calloc((strlen(A) + difference_int) , sizeof(char));  // 두 수의 자릿수가 차이나는 만큼 0으로 채울 공간
    char *temp = (char*)calloc(sizeof(char), CALLOCSIZE);
    char *temp2 = (char*)calloc(sizeof(char), CALLOCSIZE);

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
char* fill_zero_dec(char *A, char *B) {  // 덧셈, 뺄셈시 소수부 자리수가 차이나는 만큼 0으로 채움
    int int_A = find_dot(A);  // A의 정수부 길이
    int int_B = find_dot(B);  // B의 정수부 길이
    int dec_A = strlen(A) - int_A;  // A의 소수부 길이
    int dec_B = strlen(B) - int_B;  // B의 소수부 길이
    int difference_dec = dec_A - dec_B;
    if (difference_dec <= 0) return B;
    char *temp_A = A;  // A를 temp_A에 복사
    char *temp_B = B;  // B를 temp_B에 복사
    char *temp = (char*)calloc((strlen(A) + difference_dec), sizeof(char));  // 두 수의 자릿수가 차이나는 만큼 0으로 채울 공간
    char *temp2 = (char*)calloc((strlen(A) + difference_dec), sizeof(char));  // 두 수의 자릿수가 차이나는 만큼 0으로 채울 공간

    if (difference_dec > 0)   // A의 소수부가 더 긴 경우
        for (; difference_dec > 0; difference_dec--)
            strcat(temp, "0");
    strcat(temp2, temp_B);
    strcat(temp2, temp);
    return temp2;
}
int find_dot(char* num) {  // 소수점까지의 length(정수부 길이)를 찾아 return함
    int len_of_number = strcspn(num, ".");
    return len_of_number;
}
int push(int t) {
    if (top >= size - 1) {
        printf("\nStack overflow.");
        return -1;
    }
    stack[++top] = t;
    return t;
}
int pop() {
    if (top < 0) {
        printf("\nStack underflow.");
        return -1;
    }
    return stack[top--];
}
int is_legal(char *s) {
    int par = 0; //괄호 확인
    int op = 0; // 연산자 갯수 확인
    while (isspace(*s)) s++; //처음 공백 지우기
    if (*s == '-') return 0; //처음 input이 '-'면 illegal
    while (*s) {
        if (*s == ')') {
            par--;
            if (par < 0) return 0;
            s++;
        }
        else if (*s == '(') {
            par++;
            s++;
            while (isspace(*s)) s++;
            if (is_operator(*s)) {
                if (*s != '-') return 0;
                else op++;
            }
        }
        else if (is_operator(*s)) {
            op--;
            s++;
            while (isspace(*s)) s++;
            if (is_operator(*s)) return 0;
        }
        else if (isspace(*s)) s++;
        else if (*s == '.') {
            s++;
        }
        else if (isdigit(*s)) {
            s++;
            if (!isdigit(*s) && *s != '.') op++;
        }
        else return 0;
    }
    return (par == 0 && op == 1);
}
int precedence(int op) {
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    else return 3;
}
int postfix(char *dst, char *src) {
    // operator 다음에 '-' 면 다음 숫자가 operator
    int count = 0, max = 0, op;
    init_stack();
    while (isspace(*src)) src++; //처음 공백 지우기
    if (*src == '-') {
        *dst++ = '-';    // 맨 처음 input이 '-'면 음수로 처리
        src++;
        count++;
    }
    while (*src) {
        if (*src == '(') {
            push(*src);
            src++;
            while (isspace(*src)) src++;
            if (*src == '-') { //괄호 바로 뒤에 오는 '-'는 음수 처리 (not operand)
                *dst++ = '-';
                src++;
            }
        }
        else if (*src == ')') {
            while (get_top() != '(') {
                *dst++ = pop();
                *dst++ = ' ';
            }
            pop();
            src++;
        }
        else if (is_operator(*src)) {
            op = *src++;
            while (isspace(*src)) src++;
            if (is_operator(*src)) {
                *dst++ = op;
                count++;
            }
            else {
                while (!is_empty() &&
                    precedence(get_top()) >= precedence(op)) {
                    *dst++ = pop();
                    *dst++ = ' ';
                }
                push(op);
            }
        }
        else if (*src >= '0' && *src <= '9' || *src == '.') {
            do {
                *dst++ = *src++;
                count++;
            } while (*src >= '0' && *src <= '9' || *src == '.');
            if (count > max) max = count;
            count = 0;
            *dst++ = ' ';
        }
        else {
            src++;
        }
    }
    while (!is_empty()) {
        *dst++ = pop();
        *dst++ = ' ';
    }
    dst--;
    *dst = 0;
    return max;
}
void calc(char *p, int len) {
    printf("len = %d\n", len);
    char *A = malloc(len + 1);
    char *B = malloc(len + 1);
    char *answer = malloc((2 * sizeof(char)*len) + 1);
    memset(A, 0, sizeof(char)*len + 1);
    memset(B, 0, sizeof(char)*len + 1);
    memset(answer, 0, (2 * sizeof(char)*len) + 1);

    int i = 0;
    if (*p == '-') A[i++] = *p++; // 맨 처음 input이 '-'면 음수로 처리
    do {
        A[i++] = *p++;
    } while (*p >= '0' && *p <= '9' || *p == '.');
    i = 0;
    while (*p) { // printf("LINE 1036 p : %s , A : %s , B : %s\n", p, A, B);
        if (*p >= '0' && *p <= '9' || *p == '.') {
            do {
                B[i++] = *p++;
            } while (*p >= '0' && *p <= '9' || *p == '.');
        }
        else if (*p == '+') {
            p++;
            if (isdigit(*p)) B[i++] = '-'; //'-'가 operand가 아니라 음수인 경우
            answer = cal_plus(A, B);
            if(PRINT_ON) print_in_calc(A, B, answer);
            memset(A, 0, len + 1);
            memset(B, 0, len + 1);
            A = answer;
        }
        else if (*p == '*') {
            p++;
            if (isdigit(*p)) B[i++] = '-';
            answer = cal_multiply(A, B);
            if(PRINT_ON) print_in_calc(A, B, answer);
            memset(A, 0, len + 1);
            memset(B, 0, len + 1);
            A = answer;
        }
        else if (*p == '-') {
            p++;
            if (isdigit(*p)) B[i++] = '-';
            else {
                answer = cal_minus(A, B);
                if(PRINT_ON) print_in_calc(A, B, answer);
                memset(A, 0, len + 1);
                memset(B, 0, len + 1);
                A = answer;
            }
        }
        else if (*p == '/') {
            p++;
        }
        else {
            i = 0;
            p++;
        }
    }
    printf("final_answer = %s\n", answer);
    if(!COMPILE_AT_XCODE) {
        free(A);
        free(B);
        free(answer);
    }
}

void print_in_calc(char* A, char* B, char* answer) {
    printf("A = %s , B = %s , answer = %s\n", A, B, answer);
}
