#include <stdio.h>

int main() {
  //Encode To Base64
  char* base64EncodeOutput, *text="Hello World";

  Base64Encode(text, strlen(text), &base64EncodeOutput);
  printf("Output (base64): %s\n", base64EncodeOutput);
  free(base64EncodeOutput);

  //Decode From Base64
  char* base64DecodeOutput;
  size_t test;
  Base64Decode("SGVsbG8gV29ybGQ=", &base64DecodeOutput, &test);
  printf("Output: %.*s %d\n", test, base64DecodeOutput, test);
  free(base64DecodeOutput);
  


  {
	char *e = base64_encode(text, strlen(text));
	printf("Output (base64): %s\n", e);
	free(e);
  }
  {
	char *d = NULL;
	int i = base64_decode("SGVsbG8gV29ybGQ=", strlen("SGVsbG8gV29ybGQ="), &d);
	printf("Output: %.*s %d\n", i, d, i);
	free(d);
  }

  return(0);
}
