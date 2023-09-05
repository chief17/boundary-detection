#include <SoftwareSerial.h>
#include <ArduinoJson.h>
class nodeNameNumberMap
{
  public:
  char id;
  unsigned short num;
  nodeNameNumberMap *next = NULL;
};

char myName = 'b';
char myNext[2] = {'a','c'};
unsigned short numNodes = 0;
nodeNameNumberMap *startMap = NULL;
unsigned short adj[6][6] = {{0,1,1,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
bool loopDetected = false;
SoftwareSerial radio(10, 11); // RX, TX
const char myId = "b";
unsigned long lastHi = 0;

void addNodeToMap(char nodeid)
{
  nodeNameNumberMap *nextitem = startMap;
  nodeNameNumberMap *newnode = new nodeNameNumberMap();
  newnode->id = nodeid;
  newnode->num = ++numNodes;
  newnode->next=NULL;
  if(startMap == NULL)
  {
	startMap = newnode;
  }
  else
  {
    
	while(nextitem->next != NULL)
	{
  	nextitem = nextitem->next;
	}
	nextitem->next = newnode;
    
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
	; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("HardSerial Started.");

  // set the data rate for the SoftwareSerial port
  radio.begin(9600);
  Serial.println("SoftSerial Started.");
  //radio.print("{\"source\": \"b\",\"msg\": \"hi\"}");
  addNodeToMap(myName);
  addNodeToMap('a');
  addNodeToMap('c');
 
  radio.println("{\"source\": \"b\",\"msg\": \"ac\"}");
}
void printNodeMap()
{
  nodeNameNumberMap *nextitem = startMap;
  Serial.print("PrintMap: Map List uptill now: ");
  while(nextitem != NULL)
  {   
	Serial.print(nextitem->id);
	nextitem = nextitem->next;
    
  }
  Serial.println("");
}
void loop() {
  // put your main code here, to run repeatedly:
 
 
  if (radio.available()) {
	parsePacket((String)radio.readString());  // assuming we recieve {"source": "a","msg": "fb"}

	//Serial.write(radio.read());
  }
  if (Serial.available()) {
	radio.write(Serial.read());
  }
}
bool isMyNeighbour(char *nodeid)
{
//  Serial.print("isMyNeighbour?:");
//  Serial.println(nodeid);
//  Serial.print("isMyNeighbour: Comparing with ");
//  Serial.print(myNext[0]);
//  Serial.print(" and ");
//  Serial.println(myNext[1]);
  if(String(nodeid).equals(String(myNext[0])) || String(nodeid).equals(String(myNext[1])))
  {//Serial.println("isMyNeighbour?: Yes");
	return true; }
  else
  {//Serial.println("isMyNeighbour?: No");
	return false; }
}
bool inNodeMap(char *source)
{
  //printNodeMap();
  nodeNameNumberMap *nextitem = startMap;
  if(startMap == NULL)
  {
	Serial.println("NodeMap Empty");
	return false;
  }
  while(nextitem->next != NULL)
  {
//	Serial.print("inNodeMap: ");
//	Serial.print(String(nextitem->id));
//	Serial.print(" and ");
//	Serial.println(String(source));
	if(String(nextitem->id).equals(String(source)))
	{
  	return true;
	}
	nextitem = nextitem->next;
  }
  return false;
}

void parsePacket(String packet)
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, packet);
  const char* source = doc["source"];
  const char* msg = doc["msg"];
  printNodeMap();
  if(!String(source).equals(String(myName)) && !isMyNeighbour(source))
  {
	Serial.println("RX a route packet from afar.");
	if(inNodeMap(source))
	{
  	Serial.println("Source is already in NodeMap");
	}
  }
  else if(!String(source).equals(String(myName)) && isMyNeighbour(source))
  {
	Serial.println("RX from Neigbour");
	if(inNodeMap(source))
	{
  	Serial.println("Source is already in NodeMap");
  	Serial.println(msg[0]);
	}
  }
}
