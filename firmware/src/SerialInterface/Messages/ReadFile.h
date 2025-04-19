#include "Message.h"
#include "../PacketHandler.h"
#include "../../Files/Files.h"

class ReadFileMessageReceiver : public MemoryMessageReciever
{
private:
  IFiles *sdFiles = nullptr;
  IFiles *flashFiles = nullptr;
public:
  ReadFileMessageReceiver(FilesImplementation<FlashLittleFS> *flashFiles, FilesImplementation<SDCard> *sdFiles, PacketHandler *packetHandler) : sdFiles(sdFiles), flashFiles(flashFiles), MemoryMessageReciever(packetHandler) {};
  void messageFinished(bool isValid) override
  {
    // filename will be in the buffer
    if (isValid)
    {
      JsonDocument doc;
      auto error = deserializeJson(doc, getBuffer());
      if (error != DeserializationError::Ok)
      {
        sendFailure(MessageId::ReadFileResponse, "Invalid JSON");
        return;
      }
      const char *path = doc["path"];
      if (!path)
      {
        sendFailure(MessageId::ReadFileResponse, "Missing path");
        return;
      }
      bool isFlash = doc["isFlash"];
      FILE *file = isFlash ? flashFiles->open(path, "rb") : sdFiles->open(path, "rb");
      if (file)
      {
        // read the file into the buffer
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        uint8_t *buffer = new uint8_t[fileSize];
        fread(buffer, 1, fileSize, file);
        fclose(file);
        file = nullptr;
        // respond with the file data
        packetHandler->sendPacket(MessageId::ReadFileResponse, buffer, fileSize);
        delete[] buffer;
      }
      else
      {
        // respond with failure
        packetHandler->sendPacket(MessageId::ReadFileResponse, nullptr, 0);
      }
    }
  }
};
