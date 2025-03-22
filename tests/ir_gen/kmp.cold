void prefixSuffixArray(char* pat, int M, int* pps) {
   int length = 0;
   int i = 1;
   pps[0] = 0;
   
   while (i < M) {
      if (pat[i] == pat[length]) {
         length++;
         pps[i] = length;
         i++;
      } else {
         if (length != 0)
         length = pps[length - 1];
         else {
            pps[i] = 0;
            i++;
         }
      }
   }
}
void KMPAlgorithm(char* text, char* pattern) {
   int M = 10;
   int N = 10;
   int pps[M];
   int i = 0;
   int j = 0;
   prefixSuffixArray(pattern, M, pps);
   
   while (i < N) {
      if (pattern[j] == text[i]) {
         j++;
         i++;
      }
      if (j == M) {
        
         j = pps[j - 1];
      }
      else if (i < N && pattern[j] != text[i]) {
         if (j != 0)
         j = pps[j - 1];
         else
         i = i + 1;
      }
   }
}

int main() {
   char text[] = "xyztrwqxyzfg";
   char pattern[] = "xyz";
   
   KMPAlgorithm(text, pattern);
   return 0;
}