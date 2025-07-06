import unittest, json

# Simulated SSE parser based on OllamaClient::sendChatRequest logic

def parse_sse(chunks, on_chunk):
    buffer = ""
    stop = False
    for data in chunks:
        buffer += data.replace("\r\n", "\n")
        while True:
            pos = buffer.find("\n\n")
            if pos == -1:
                break
            event = buffer[:pos]
            buffer = buffer[pos + 2:]
            if event.startswith("data: "):
                payload = event[6:]
                if payload == "[DONE]":
                    stop = True
                    break
                try:
                    obj = json.loads(payload)
                    if not on_chunk(obj):
                        stop = True
                        break
                except json.JSONDecodeError:
                    pass
        if stop:
            break
    return not stop

class TestSSEParser(unittest.TestCase):
    def test_crlf_events(self):
        sse = (
            "data: {\"id\":1}\r\n\r\n"
            "data: {\"id\":2}\r\n\r\n"
            "data: [DONE]\r\n\r\n"
        )
        chunks = [sse[:10], sse[10:20], sse[20:]]
        received = []
        parse_sse(chunks, lambda j: (received.append(j) or True))
        self.assertEqual(received, [{"id":1}, {"id":2}])

if __name__ == "__main__":
    unittest.main()