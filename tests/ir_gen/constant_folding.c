// // int main()
// // {
// //     int and_result = 252645135 & 16711935; // 983055

// //     int or_result = 252645135 | 16711935; //  268374015

// //     int xor_result = 252645135 ^ 16711935; // 267390960

// //     // Left shift: 291 << 18 = 76283904
// //     int shift_left_result = 291 << 18;

// //     // Right shift: 252645135 >> 9 = 493447
// //     int shift_right_result = 252645135 >> 9;
// // }

// // arithmetic tests
// int target_add(void) {
//     return 100 + 200;  // 300
// }

// int target_sub(void) {
//     return 2 - 2147483647;  // -2147483645
// }

// int target_mult(void) {
//     return 1000 * 1000;  // 1000000
// }

// int target_div(void) {
//     return 1111 / 4;  // 277
// }

// int target_rem(void) {
//     return 10 % 3;  // 1
// }

// // relational tests
// int target_eq_true(void) {
//     return 2147483647 == 2147483647;  // 1
// }

// int target_eq_false(void) {
//     return 2147483647 == 2147483646;  // 0
// }

// int target_negate(void) {
//     return -3;
// }

// int target_negate_zero(void) {
//     return -0;
// }

// int target_not(void) {
//     return !1024;
// }

// int target_not_zero(void) {
//     return !0;
// }

// int target_complement(void) {
//     return ~1;
// }
