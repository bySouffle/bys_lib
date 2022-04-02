//
// Created by bys on 2022/4/2.
//

#include "hv/HttpServer.h"
#include "gtest/gtest.h"
#include "hv/hv.h"
#include "curl/curl.h"
#include "tinyxml2.h"

TEST(hv, test) {

#if 1

  HttpService router;
  router.GET("/ping", [](HttpRequest *req, HttpResponse *resp) {
    return resp->String("pong");
  });

  router.GET("/data", [](HttpRequest *req, HttpResponse *resp) {
    static char data[] = "0123456789";
    return resp->Data(data, 10);
  });

  router.GET("/paths", [&router](HttpRequest *req, HttpResponse *resp) {
    return resp->Json(router.Paths());
  });

  router.GET("/get", [](HttpRequest *req, HttpResponse *resp) {
    resp->json["origin"] = req->client_addr.ip;
    resp->json["url"] = req->url;
    resp->json["args"] = req->query_params;
    resp->json["headers"] = req->headers;
    return 200;
  });

  router.POST("/echo", [](const HttpContextPtr &ctx) {
    return ctx->send(ctx->body(), ctx->type());
  });

  http_server_t server;
  server.port = 8080;
  server.service = &router;
  http_server_run(&server);

  while (1) {

  }
#endif
}

size_t get_recv_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  std::string data((const char *) ptr, (size_t) size * nmemb);
  *((std::stringstream *) stream) << data << std::endl;
  return size * nmemb;
}

TEST(curl_put, test) {

  std::stringstream out;

  void *curl = curl_easy_init();

  if (curl) {
    curl_slist *headers = curl_slist_append(nullptr, "Content-Type: application/xml");
    headers = curl_slist_append(headers, "Authorization: Basic YWRtaW46YWRtaW4xMjM=");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.67:80/PSIA/PTZ/channels/1/continuous");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

//    std::string val(R"~(<?xml version" = ""1.0" encoding="UTF-8"?>
//        <PTZData><pan>30</pan><tilt>0</tilt></PTZData>)~");


    std::string val(R"~(<?xml version" = ""1.0" encoding="UTF-8"?>
    <PTZData><zoom>60</zoom></PTZData>)~");

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, val.data()); /* data goes here */

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_recv_data);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);

    CURLcode res = curl_easy_perform(curl);

    std::cout << out.str() << "\n";
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    tinyxml2::XMLDocument xml;
//    xml.Parse(out.str().c_str());

    std::string test_str(R"~(<?xml version="1.0" encoding="UTF-8" ?>
<ResponseStatus version="1.0" xmlns="urn:psialliance-org">
<requestURL>/PSIA/PTZ/channels/ID/continuous</requestURL>
<statusCode>1</statusCode>
<statusString>OK</statusString>
</ResponseStatus>)~");
    xml.Parse(test_str.data());

    tinyxml2::XMLElement *root = xml.FirstChildElement("ResponseStatus");
    tinyxml2::XMLElement *statusCode = root->FirstChildElement("statusCode");
    std::cout << statusCode->GetText() << "\n";
//    cout << "statusCode: " << xml.FirstChildElement("statusCode")->GetText() << "\n";
//    cout << "statusString: " << xml.FirstChildElement("statusString")->GetText();

  }
}

TEST(xml_parse, test) {
  tinyxml2::XMLDocument xml;

  std::string test_str(R"~(<?xml version="1.0" encoding="UTF-8" ?>
<ResponseStatus version="1.0" xmlns="urn:psialliance-org">
<requestURL>/PSIA/PTZ/channels/ID/continuous</requestURL>
<statusCode>1</statusCode>
<statusString>OK</statusString>
</ResponseStatus>)~");
  xml.Parse(test_str.data());

  tinyxml2::XMLElement *root = xml.FirstChildElement("ResponseStatus");
  tinyxml2::XMLElement *statusCode = root->FirstChildElement("statusCode");
  std::cout << statusCode->GetText() << "\n";

//    cout << "statusCode: " << xml.FirstChildElement("statusCode")->GetText() << "\n";
  std::cout << "statusString: " << root->FirstChildElement("statusString")->GetText();
}
}