#include <iostream>   
#include <string>    
#include <iomanip>   
#include <fstream>  

using namespace std;

const int MAX_MENU_ITEMS = 100;
const int MAX_ORDER_ITEMS = 100;
const int MAX_TABLES = 20;
const int MAX_WAITERS = 10;

class MenuItem {
	
    private:
        string name;
        string description;
        double price;

    public:
    	
        MenuItem(string n, string desc, double p) : name(n), description(desc), price(p) {}  
        MenuItem() : name(""), description(""), price(0.0) {}

    void display() const {
    	
        cout<<setw(20)<<left<<name<<setw(30)<<left<<description<<setw(10)<<left<<price<<endl;
    }

    double getPrice() const {
    	
        return price;
    }

    string getName() const {
    	
        return name;
    }

    string getDescription() const {
    	
        return description;
    }
};

class Menu {
    private:
    	
        MenuItem items[MAX_MENU_ITEMS];
        int itemCount;

    public:
    	
    Menu() : itemCount(0) {} 

    void addItem(const MenuItem& item) {
    	
        if (itemCount < MAX_MENU_ITEMS) {
        	
            items[itemCount++] = item;
            
        } 
        
		else {
			
            cerr<<"Menu is full. Cannot add more items."<<endl;
            
        }
    }

    void display() const {
    	
        cout<<setw(20)<<left<<"Name"<<setw(30)<<left<<"Description"<<setw(10)<<left<<"Price"<<endl;
        cout<<setfill('-')<<setw(60)<<"-"<<setfill(' ')<<endl;
        
        for (int i = 0; i < itemCount; ++i) {
        	
            items[i].display();
            
        }
    }

    MenuItem getItem(int index) const {
    	
        if (index >= 1 && index <= itemCount) {
        	
            return items[index - 1];
            
        } 
		else {
			
            cout<<"Invalid menu item index"<<endl;
            return MenuItem();
        }
    }

    void saveToFile(const string& filename) const {
    	
        ofstream file(filename.c_str());
        
        if (file.is_open()) {
        	
            for (int i = 0; i < itemCount; ++i) {
            	
                file<<items[i].getName()<<"\n"<<items[i].getDescription()<<"\n"<< items[i].getPrice()<<"\n";
                
            }
            
            file.close();
            
        } 
		else {
			
            cerr<<"Unable to open file for writing."<<endl;
            
        }
    }

    void loadFromFile(const string& filename) {
    	
        ifstream file(filename.c_str());
        
        if (file.is_open()) {
        	
            itemCount = 0;
            string name, description;
            double price;
            
            while (getline(file, name) && getline(file, description) && file >> price) {
            	
                file.ignore(); // to ignore the newline character after reading the price
                addItem(MenuItem(name, description, price));
            }
            file.close();
        } 
		else {
			
            cerr<<"Unable to open file for reading."<<endl;
        }
    }
};

class Order {
	
    private:
    	
    struct OrderItem {   // structure
    	
        MenuItem item;
        int quantity;
        OrderItem(const MenuItem& itm, int qty) : item(itm), quantity(qty) {}
        OrderItem() : item(), quantity(0) {}
        
    };

    OrderItem orderItems[MAX_ORDER_ITEMS]; //array
    
    int orderCount;

    public:
	
    Order() : orderCount(0) {}  //default constructor 

    void addItem(const MenuItem& item, int quantity) {
    	
        if (orderCount < MAX_ORDER_ITEMS) {
        	
            orderItems[orderCount++] = OrderItem(item, quantity);
            
        } 
		else {
			
            cerr<<"Order is full. Cannot add more items."<<endl;
            
        }
    }

    void display() const {
    	
        cout<<setw(20)<<left<<"Name"<<setw(10)<<left<<"Price"<<setw(10)<<left<<"Quantity"<<setw(10)<<left<<"Total"<< endl;
        cout<<setfill('-')<<setw(50)<<"-"<<setfill(' ')<<endl;
        
		for (int i = 0; i < orderCount; ++i) {
			
            cout<<setw(20)<<left<<orderItems[i].item.getName()<<setw(10)<<left<<orderItems[i].item.getPrice()<<setw(10)<<left<<orderItems[i].quantity<<setw(10)<<left<<orderItems[i].item.getPrice() * orderItems[i].quantity<<endl;
        }
    }

    double getTotalBill(double taxRate = 0.0, double discount = 0.0) const {
    	
        double total = 0;
        
        for (int i = 0; i < orderCount; ++i) {
        	
            total += orderItems[i].item.getPrice() * orderItems[i].quantity;
            
        }
        
        total += total * taxRate;
        total -= discount;
        
        return total;
    }
};

class Table {
	
    private:
       int tableNumber;
       bool occupied;
       bool reserved;
       Order order;

    public:
    	
    Table() : tableNumber(0), occupied(false), reserved(false) {} 
    
    Table(int number) : tableNumber(number), occupied(false), reserved(false) {} 

    int getTableNumber() const {
    	
        return tableNumber;
        
    }

    bool isOccupied() const {
    	
        return occupied;
        
    }

    bool isReserved() const {
    	
        return reserved;
        
    }

    Order& getOrder() {
    	
        return order;
        
    }

    void occupy() {
    	
        occupied = true;
        
    }

    void vacate() {
    	
        occupied = false;
        reserved = false;
        order = Order(); 
        
    }

    void reserve() {
    	
        reserved = true;
        
    }

    void displayOrder() const {
    	
        cout<<"Table "<<tableNumber<<" Order Summary:"<<endl;
        order.display();
        
    }

    double getBill(double taxRate = 0.0, double discount = 0.0) const {
    	
        return order.getTotalBill(taxRate, discount);
        
    }
};

class User {
	 
    protected:
       string username;
       string password;

public:
	
    User(string uname, string pass) : username(uname), password(pass) {}  
    virtual ~User() {}

    string getUsername() const {
    	
        return username;
        
    }

    bool authenticate(const string& uname, const string& pass) const {
    	
        return username == uname && password == pass;
        
    }

    virtual void menuActions(Menu& menu, Table tables[], int& tableCount) = 0;
};

class Admin : public User {
	
    public:
    	
    Admin(string uname, string pass) : User(uname, pass) {}

    void menuActions(Menu& menu, Table tables[], int& tableCount) override {
    	
        int choice;
        
        do {
            cout<<"\nAdmin Menu:\n";
            cout<<"1. Add Menu Item\n";
            cout<<"2. Display Menu\n";
            cout<<"3. Save Menu to File\n";
            cout<<"4. Load Menu from File\n";
            cout<<"5. View Tables\n";
            cout<<"6. View Total Earnings\n";
            cout<<"7. Exit Admin Menu\n";
            cout<<"Enter your choice: ";
            cin>>choice;

            switch (choice) {
            	
                case 1: {
                	
                    string name, description;
                    double price;
                    cout<<"Enter Food name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout<<"Enter Food description: ";
                    getline(cin, description);
                    cout<<"Enter Food price: ";
                    cin>>price;
                    menu.addItem(MenuItem(name, description, price));
                    break;
                    
                }
                case 2:
                	
                    menu.display();
                    break;
                    
                case 3: {
                	
                    string filename;
                    cout<<"Enter filename to save menu: ";
                    cin>>filename;
                    menu.saveToFile(filename);
                    break;
                    
                }
                case 4: {
                	
                    string filename;
                    cout<<"Enter filename to load menu: ";
                    cin>>filename;
                    menu.loadFromFile(filename);
                    break;
                    
                }
                case 5: {
                	
                    cout<<"Tables:\n";
                    for (int i = 0; i < tableCount; ++i) {
                        cout<<"Table "<<tables[i].getTableNumber()<<": "<<(tables[i].isOccupied() ? "Occupied" : "Vacant")<<(tables[i].isReserved() ? " (Reserved)" : "")<<endl;
                    }
                    break;
                }
                
                case 6: {
                	
                    double totalEarnings = 0.0;
                    for (int i = 0; i < tableCount; ++i) {
                        if (tables[i].isOccupied() || tables[i].isReserved()) {
                            totalEarnings += tables[i].getBill(0.10); // Assume a 10% tax rate
                        }
                    }
                    
                    cout<<"Total Earnings so far: $"<<totalEarnings<<endl;
                    break;
                }
                
                case 7:
                    cout<<"Exiting Admin Menu\n";
                    break;
                    
                default:
                    cout<<"Invalid choice. Try again.\n";
            }
        } 
    while (choice != 7);

    }
};

class Waiter : public User {
	
public:
	
    Waiter(string uname, string pass) : User(uname, pass) {}   

    void menuActions(Menu& menu, Table tables[], int& tableCount) override {
    	
        int choice;
        
        do {
            cout<<"\nWaiter Menu:\n";
            cout<<"1. View Tables\n";
            cout<<"2. Take Order\n";
            cout<<"3. Print Bill\n";
            cout<<"4. Exit Waiter Menu\n";
            cout<<"Enter your choice: ";
            cin>>choice;

            switch (choice) {
            	
                case 1: {
                    cout<<"Tables:\n";
                    for (int i = 0; i < tableCount; ++i) {
                        cout<<"Table "<<tables[i].getTableNumber()<<": "<<(tables[i].isOccupied() ? "Occupied" : "Vacant")<<(tables[i].isReserved() ? " (Reserved)" : "") << endl;
                    }
                    break;
                }
                
                case 2: {
                    int tableNumber;
                    cout<<"Enter table number to take order (1-" << MAX_TABLES << "): ";
                    cin>>tableNumber;
                    
                    if (tableNumber < 1 || tableNumber > MAX_TABLES) {
                        cout<<"Invalid table number. Exiting."<<endl;
                        return;
                    }
                    
                    Table& table = tables[tableNumber - 1];
                    
                    if (!table.isOccupied()) {
                        cout<<"Table "<<tableNumber<<" is not occupied."<<endl;
                        return;
                    }

                    int itemNumber, quantity;
                    
                    cout<<"Here is the menu:\n";
                    menu.display(); 
                    cout<<"Enter the item number you'd like to order (0 to finish): ";
                    
                    while (cin>>itemNumber && itemNumber != 0) {
                    	
                        cout<<"Enter quantity: ";
                        cin>>quantity;
                        MenuItem item = menu.getItem(itemNumber);
                        
                        if (item.getName() != "") {
                            table.getOrder().addItem(item, quantity);
                            cout<<"Added "<<quantity<<"x "<<item.getName()<<" to the order."<<endl;
                        } else {
                            cout<<"Invalid item number. Please try again."<<endl;
                        }
                        cout<<"Enter the item number you'd like to order (0 to finish): ";
                    }
                    cout<<"Order taken successfully for Table "<<tableNumber<<"."<<endl;
                    break;
                }
                
                case 3: {
                    int tableNumber;
                    cout<<"Enter table number to print bill (1-"<< MAX_TABLES <<"): ";
                    cin>>tableNumber;
                    
                    if (tableNumber < 1 || tableNumber > MAX_TABLES) {
                        cout<<"Invalid table number. Exiting."<<endl;
                        return;
                    }
                    
                    Table& table = tables[tableNumber - 1];
                    
                    if (!table.isOccupied()) {
                        cout<<"Table "<<tableNumber<<" is not occupied."<<endl;
                        return;
                    }
                    table.displayOrder();
                    double bill = table.getBill(0.10); 
                    cout<<"Total Bill (including tax): $"<<bill<<endl;
                    break;
                }
                
                case 4:
                    cout<<"Exiting Waiter Menu\n";
                    break;
                    
                default:
                    cout<<"Invalid choice. Try again.\n";
            }
            
        }
		 while (choice != 4);
    }
};

class Customer : public User {
	
public:
	
    Customer(string uname, string pass) : User(uname, pass) {}

    void menuActions(Menu& menu, Table tables[], int& tableCount) override {
    	
        int choice;
        cout<<"Welcome! Please choose an option:\n";
        cout<<"1. Reserve a table\n";
        cout<<"2. Dine in\n";
        cout<<"Enter your choice: ";
        cin>>choice;

        switch (choice) {
        	
            case 1: {
            	
                int tableNumber;
                cout<<"Enter table number to reserve (1-"<< MAX_TABLES <<"): ";
                cin>>tableNumber;
                
                if (tableNumber < 1 || tableNumber > MAX_TABLES) {
                    cout<<"Invalid table number. Exiting."<<endl;
                    return;
                }
                
                Table& table = tables[tableNumber - 1];
                
                if (table.isReserved() || table.isOccupied()) {
                    cout<<"Sorry, the table is not available for reservation. Please choose another table or dine in."<<endl;
                    return;
                }
                table.reserve();
                
                cout<<"Table "<<tableNumber<<" reserved successfully!"<<endl;
                break;
            }
            
            case 2: {
                int tableNumber;
                cout<<"Enter table number to dine in (1-"<< MAX_TABLES<< "): ";
                cin>>tableNumber;
                
                if (tableNumber < 1 || tableNumber > MAX_TABLES) {
                    cout<<"Invalid table number. Exiting."<<endl;
                    return;
                }
                
                Table& table = tables[tableNumber - 1];
                
                if (table.isOccupied()) {
                    cout<<"Sorry, the table is already occupied. Please choose another table."<<endl;
                    return;
                }
                
                table = Table(tableNumber); 
                table.occupy();
                cout<<"You are now seated at Table "<<tableNumber<<"."<<endl;

                cout<<"Please call a waiter to take your order."<<endl;
                
                break;
            }
            
            default:
                cout<<"Invalid choice. Exiting."<< endl;
        }
    }
};

int main() {
	
    cout<<"\t\t\t\tRestaurant Management System"<<endl;

    Menu menu;
    
    menu.addItem(MenuItem("Burger", "Classic beef burger", 9.99));
    menu.addItem(MenuItem("Pizza", "Margherita pizza", 12.99));
    menu.addItem(MenuItem("Pasta", "Spaghetti pasta with marinara sauce", 8.99));
    menu.addItem(MenuItem("Salad", "Fresh garden salad", 5.99));

    Table tables[MAX_TABLES];
    
    for (int i = 0; i < MAX_TABLES; ++i) {
        tables[i] = Table(i + 1); 
    }
    
    int tableCount = MAX_TABLES;

    bool exitProgram = false;

    do {
    	
        int userType;
        cout<<"Enter 1 for Admin, 2 for Waiter, or 3 for Customer: ";
        cin>>userType;

        User* user = NULL;
        
        if (userType == 1) {
            user = new Admin("admin", "password");
            string username, password;
            cout<<"Enter admin username: ";
            cin>>username;
            cout<<"Enter admin password: ";
            cin>>password;

            if (user->authenticate(username, password)) {
                user->menuActions(menu, tables, tableCount);
            } 
			else {
                cout<<"Authentication failed. Exiting."<<endl;
                
                delete user;
                return 1;
            }
        }
	    else if (userType == 2) {
	    	
            user = new Waiter("waiter", "password");
            string username, password;
            cout<<"Enter waiter username: ";
            cin>>username;
            cout<<"Enter waiter password: ";
            cin>>password;

            if (user->authenticate(username, password)) {
            	
                user->menuActions(menu, tables, tableCount);
                
            } 
			else {
				
                cout<<"Authentication failed. Exiting."<<endl;
                
                delete user;
                return 1;
                
            }
        } 
		else if (userType == 3) {
			
            user = new Customer("customer", "password");
            user->menuActions(menu, tables, tableCount);
            
        } 
		else {
			
            cout<<"Invalid user type. Exiting."<<endl;
            return 1;
        }

        delete user;

    
        
        char choice;
        cout<<"Do you want to continue? (Y/N): ";
        cin >> choice;
        
        if (choice == 'N' || choice == 'n') {
            exitProgram = true;
        }
    }
	while (!exitProgram);

    return 0;
}

