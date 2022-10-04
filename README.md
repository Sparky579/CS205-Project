# CS205-ProjectProject 1: A Simple Calculator

Part 1 – Analysis


	需求：实现一个可以完成实数乘法，并且支持科学计数法和异常处理。常规的数字与C++标准输入一样，如100,3.14等。科学计数法的数字中间含有e，“XeY”表示X*10^Y，如3e4表示整数30000。另外，输入中可能还有含有异常数据，比如“1e200”（过大的数据），以及格式错误的数据。所以，不难分析得出普通的int或者double都难以满足要求，需要利用字符串手动实现数字的转化。
	输入输出格式
	输入：./mul a b
	输出：a * b = c，或者报出异常。

Part 2 – Code
	由于本项目代码过长（共计240行），代码另外附在提交中。

Part 3 - Result & Veriﬁcation
简略：Case 1~6为基本样例，Case 7~8为高精度浮点数处理，Case 9~16为各种非常规输入处理。 
Test case #1:

Input: ./mul 2 3
Output: 2 * 3 = 6

Test case #2:

Input: ./mul 3.1416 2
Output: 3.1416 * 2 = 6.2832

Test case #3:

Input: ./mul 3.1415 2.0e-2
Output: 3.1415 * 0.02 = 0.06283

 
Test case #4:
Input: ./mul a 2
Output: The input cannot be interpret as numbers! 
  
Test case #5:
Input: ./mul 1234567890 1234567890

Output: 1234567890 * 1234567890 = 1524157875019052100
 
Test case #6:
Input: ./mul 1.0e200 1.0e200
Output: 10000…(200 0s) * 10000…(200 0s) = 10000(400 0s)
 
For my program, these two cases can be calculated instead of throwing an exception!
(以下为额外构造的测试数据)
Test case #7:
Input: ./mul 1.23e3 3.1234567898765 
Output: 1230 * 3.1234567898765 = 3841.851851548095


Test case #8:
Input: ./mul 1.23e-3 -3e3
Output: 0.00123 * -3000 = -3.69

Test case #9:
Input: ./mul 3
Output: Too few input! 

 



Test case #10:
Input: ./mul 3 3 3
Output: Too many inputs!
  

Test case #11:
Input: ./mul 3.e3 3
Output: The input cannot be interpret as numbers!

Test case #12:
Input: ./mul 3e3.3 3
Output: The input cannot be interpret as numbers! 

Test case #13:
Input: ./mul 3e 3
Output: The input cannot be interpret as numbers! 
 

These cases are designed to test exception throwing ability.

Test case #14:
Input: ./mul -1e0 ----2.1e-2
Output: -1 * 0.021 = -0.021
Surplus minus can also be dealt.
 
Test case #15:
Input: ./mul 1e7777 1e9999
Output: The number is too large or too precise!

Test case #16:
Input: ./mul 1e-7777 1e-9999
Output: The number is too large or too precise!
 
Highest Precision: 10000 digits of Integer Part, 10000 digits of fraction part.




Part 4 - Diﬃculties & Solutions
4.1 如何处理精度很高的数字，并且正确地显示小数位数？
利用Number结构体实现高精度数字。该结构体包含数组intPart, numPart,变量numSign, intLen, fracLen, errorCode, 分别表示整数部分的每一位数，小数部分的每一位数，整个数字的正负号，整数部分长度，小数部分长度以及错误代码（精度过高或者格式错误）。
		使用重载运算符*来将两个Number相乘。为了方便，两个数组中较低的下标为靠近小数点的数位。那么我们将两个数拆开为每一个数位的和，再用乘法分配律两重循环处理两个数每一位的积。小数位i*小数位j得到的位数为i + j + 1；整数位i*整数位j得到的位数为i + j；小数位*整数位分情况考虑得到的结果在小数位还是整数位。
		但是，乘出来的结果很可能会导致每一位的数字大于10，如3 * 5，则个位数变成了15而十位数是0，那应该如何处理呢？我们从最小的位数（小数部分最后一位）开始模拟进位。将该位数除以10得到的整数部分全部进位至高一位，一直到大于等于最高位且进位值为0。这样我们还顺便统计了新数的整数部分位数。至于小数部分，我们从i + j位小数开始，向前探测若发现0则舍弃末尾的0。这样，我们可以保证最后输出的格式中没有前导0，也没有末尾0。
 4.2 如何处理科学计数法的数字（中间由e分隔，如1e9,1.5e-6）
检测中间e的数字，记录e的位置后分别对前后段进行处理。后段只能出现开头的一个-号和数字。因为最多10000位，用一个int变量记录e后面的值。前半段数字通过小数点分隔记录整数位和小数位，再整体左移e后面的值的位数。
4.3 如何处理输入输出的各种异常状况？ 
输入方面，首先是与小数点相关的异常。小数点若出现在最后，由于小数部分初始为0，检测不到小数点后的数字输入，直接被判定为整数。如“5.”被识别为整数5，与C++内部输入逻辑相仿。但如果只有小数点或者小数点个数超过一个，直接抛出异常；其次是字母e相关的异常。若出现超过一个e，直接抛出异常；若e出现在开头或者结尾也直接抛出异常；接下来是关于负号，负号只允许出现在开头和字母e后面，其中开头若出现多个负号，依次删去每个并且反转结构体的符号标记。在这些情况之外出现的非0~9数字直接令程序抛出异常。
输出方面的异常主要有：整数部分为空，特殊标记整数部分长度为0，并且输出一个0而不是什么都不输出；小数部分为空，直接不输出小数点。 



 

