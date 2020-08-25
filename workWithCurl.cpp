#include "workWithCurl.h"
#include <fstream>
#include "clearSky.h"

#pragma comment(lib, "libcurl.lib")
#include <curl/curl.h>
#include "screenshot.h"
void API_sendFromDiskC(std::string url) {
	API_screenToPNG(L"C:/temp.png");
	send(url, "C:/temp.png");
}
void send(std::string url, std::string pathToImage) {


	std::string contents;
	std::ifstream in(pathToImage, std::ios::in | std::ios::binary);

	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
	//##


	CURL* curl;
	CURLcode res;

	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;
	struct curl_slist* headerlist = NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);

	// set up the header
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "cache-control:",
		CURLFORM_COPYCONTENTS, "no-cache",
		CURLFORM_END);

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "content-type:",
		CURLFORM_COPYCONTENTS, "multipart/form-data",
		CURLFORM_END);

	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "file", // <-— the (in this case) wanted file-Tag!
		CURLFORM_BUFFER, "data",
		CURLFORM_BUFFERPTR, contents.data(),
		CURLFORM_BUFFERLENGTH, contents.size(),
		CURLFORM_END);

	curl = curl_easy_init();

	headerlist = curl_slist_append(headerlist, buf);
	if (curl) {

		curl_easy_setopt(curl, CURLOPT_URL, url.data());

		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			myLog("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));


		curl_easy_cleanup(curl);


		curl_formfree(formpost);

		curl_slist_free_all(headerlist);

		myLog("Good send.");
	}
	else {
		myLog("not curl.");
	}
}