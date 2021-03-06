#include <string.h>

#include "test.h"


uint8_t spec = 0;
uint16_t test_passed = 0;
uint16_t test_failed = 0;

void _test_pass (const char *message) {
#ifdef ARDUINO
  if (spec) {
    Serial.print("  ✓ ");
    Serial.println(message);
  } else {
    Serial.print(".");
  }
#else
  if (spec) {
    printf("  ✓ %s\n", message);
  } else {
    printf(".");
  }
#endif
}

void _test_fail (const char *message, const char *file, uint16_t line) {
#ifdef ARDUINO
  if (spec) {
    Serial.print("  𝙭 (");
    Serial.print(message);
    Serial.print(":");
    Serial.print(line);
    Serial.println(")");
  } else {
    Serial.print("𝙭");
  }
#else
  if (spec) {
    printf("  𝙭 %s (%s:%d)\n", message, file, line);
  } else {
    printf("𝙭");
  }
#endif
}

void _test_start (const char *name) {
  if (spec) {
#ifdef ARDUINO
    Serial.println();
    Serial.println(name);
#else
    printf("\n%s\n", name);
#endif
  }
}

#ifdef ARDUINO
void setup ( ) {
  Serial.begin(115200);
  Serial.println("BEGIN TESTS");
  delay(25);
}
#endif

#ifdef ARDUINO
void loop ( ) {
  spec = 1;

#else
int main (int argc, char **argv) {
  if (argc > 1) {
    if ((strcmp(argv[1], "--spec") == 0) || (strcmp(argv[1], "-s") == 0)) {
      spec = 1;
    }
  }

#endif

  test(test_parse_params, "Parse Params");
  test(test_parse_headers, "Parse Headers");
  test(test_parse_string, "Parse String");

  test(test_response_write, "Response Write");

  test(test_routing, "Routing");


#ifdef ARDUINO
  Serial.println();
  Serial.print("PASSED: ");
  Serial.println(test_passed);
  Serial.print("FAILED: ");
  Serial.println(test_failed);

  delay(30000);
#else
  printf("\nPASSED: %d\nFAILED: %d\n", test_passed, test_failed);

  return (test_failed > 0 ? 1 : 0);
#endif
}
