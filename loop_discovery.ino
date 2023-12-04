class listitem
{
  public:
  short item;
  
  listitem *next = NULL;
};
//supposed adj matrix compiled after tx rx
short adj[5][5] = {{0,1,0,0,1},{1,0,1,0,0},{0,1,0,1,0},{0,0,1,0,1},{1,0,0,1,0}};
const short num_nodes = 5;
bool isloop = false;
listitem *startvisited = NULL;
listitem *startnodes = NULL;

//function to check if a node is already visited-------------------------------------------------------------------------
bool isVisited(short nodeid)
{
  listitem *nextitem = startvisited;
  if(startvisited == NULL)
  {
    return false;
  }
  while(nextitem->next != NULL)
    {
    if(nodeid == nextitem->item)
    {
      return true;
    }
    nextitem=nextitem->next;
  }
  return false;
}
//function to print lists--------------------------------------------------------------------------------------------------
void printVisited()
{
  listitem *nextitem = startvisited;
  Serial.print("PrintVisited: Visited List uptill now: ");
  while(nextitem != NULL)
  {   
   Serial.print(nextitem->item);
    nextitem = nextitem->next;
    
  } 
  Serial.println("");
}

//function to add a visited node to a list-------------------------------------------------------------------------------------
void addVisited(short nodeid)
{
    Serial.print("Adding to visited list: ");
    Serial.println(nodeid);
    
    listitem *nextitem = startvisited;
    listitem *newitem = new listitem();
    newitem->item = nodeid;
    newitem->next = NULL;
    if(startvisited == NULL)
    {
      //std::cout<<"List was empty."<<endl;
      startvisited = newitem;
      //return;
    }
    else
    {
      //std::cout<<"List was not empty."<<endl;
      while(nextitem->next != NULL)
        nextitem = nextitem->next;
      nextitem->next = newitem;
    }
    
}

//function to keep track of discovered nodes----------------------------------------------------------------------------
void addToList(listitem *start,short nodeid)
{
    Serial.print("Adding to visited list: ");
    Serial.println(nodeid);
    
    listitem *nextitem = start;
    listitem *newitem = new listitem();
    newitem->item = nodeid;
    newitem->next = NULL;
    if(start == NULL)
    {
      //std::cout<<"List was empty."<<endl;
      start = newitem;
      //return;
    }
    else
    {
      //std::cout<<"List was not empty."<<endl;
      while(nextitem->next != NULL)
        nextitem = nextitem->next;
      nextitem->next = newitem;
    }
    
}
bool isLastVisited(short nodeid)
{
  listitem *nextitem = startvisited;
  if(startvisited == NULL)
  {
    return false;
  }
  while(nextitem->next != NULL)
    nextitem = nextitem->next;
    
 Serial.println(sprintf("Comparing %d with %d",nodeid,nextitem->item));
  if(nextitem->item == nodeid)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//function to check breadth of a visited node
bool check_breadth(short start, short next, short row[num_nodes], short last)
{
  
    listitem node;
    listitem *startitem = &node;
    listitem *nextitem = startitem;
    for(short x = 0; x<=(num_nodes-1); x++)
    {
      if(row[x] == 1)
      {
        Serial.print("Adding to temp list: ");
        Serial.println(x);
        nextitem->item = x;
        nextitem->next = new listitem();
        nextitem = nextitem->next;
      }
    }
    nextitem = startitem;
    while(nextitem->next != NULL)
    {
      Serial.print("Considering: ");
      Serial.println(nextitem->item);
      if(nextitem->item == start && isVisited(nextitem->item) == true && !isLastVisited(nextitem->item))
      {
        Serial.println("Loop Detected");
        isloop = true;
        addVisited(next);
        printVisited();
        return true;
      }
      else if(isLastVisited(nextitem->item) == false)
      {
        addVisited(next);
        printVisited();
            Serial.print("Calling CB with item ");
            Serial.println(nextitem->item);
            if(check_breadth(0,nextitem->item,adj[nextitem->item],0))
        {
          return true;
        }
        else
          return false;
        
      } 
      else
      {
        nextitem=nextitem->next;
        
      }
      
    }
    return false;
    
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial Started");
  Serial.println("Matrix init done");
  
  if(check_breadth(0,0,adj[0],0))
    Serial.println("Success");
   else
    Serial.println("NoLoop");
}

void loop() {
  // put your main code here, to run repeatedly:

}
