/*
 * Dynomite - A thin, distributed replication layer for multi non-distributed storages.
 * Copyright (C) 2014 Netflix, Inc.
 */

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>



int
calc_decode_length(const char *b64input, const size_t length) {
	int padding = 0;

	// Check for trailing '=''s as padding
	if(b64input[length-1] == '=' && b64input[length-2] == '=')
		padding = 2;
	else if (b64input[length-1] == '=')
		padding = 1;

	return (length*3)/4 - padding;
}


// https://gist.github.com/barrysteyn/7308212
//
char*
base64_encode(const unsigned char *message, const size_t length)
{
	BIO *bio, *b64;
	BUF_MEM *messagePtr;
	
	char *buff = NULL;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, message, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &messagePtr);
	
	buff = (char*)malloc(messagePtr->length);
	memcpy(buff, messagePtr->data, messagePtr->length - 1);
	buff[messagePtr->length - 1] = '\0';

	// BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	return buff; // XXX. is this pointer use correct?
	// return ((*messagePtr).data); // XXX. is this pointer use correct?
}

#include <assert.h>

// XXX. why is this order not consistent with base64_encode?
// XXX. is this even used?
int
base64_decode(const char *b64message, const size_t length, unsigned char **buffer)
{
	BIO *bio;
	BIO *b64;

	// XXX. why is length passed around?
	int decodedLength = calc_decode_length(b64message, length);

	int l = 0;
	// XXX. do we really need to check every malloc?
	*buffer = (unsigned char*)malloc(decodedLength+1);
	assert(*buffer != NULL);

	bio = BIO_new_mem_buf((void *)b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	l = BIO_read(bio, *buffer, strlen(b64message));
	assert(l == decodedLength); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return decodedLength;
}

