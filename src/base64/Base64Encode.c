//Encodes Base64
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <stdint.h>

int Base64Encode(const uint8_t* buffer, size_t length, char** b64text) { //Encodes a binary safe base 64 string
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	char* buff = NULL;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);

	// BIO_set_close(bio, BIO_NOCLOSE);

	buff = (char*)malloc(bufferPtr->length);
	memcpy(buff, bufferPtr->data, bufferPtr->length - 1);
	buff[bufferPtr->length - 1] = '\0';

	BIO_free_all(bio);

	*b64text=buff;

	return (0); //success
}
