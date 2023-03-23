#include "my_module.h"

int main(){
    string fileName, outputFileName;
    char choice;

    while (true) {
        cout << "Do you want to create New file or open an Existing (N/E):_";
        cin >> choice;
        cin.ignore();
        if (choice == 'n' || choice == 'N') {
            cout << "Enter the name of the file without an extension:_";
            getline(cin, fileName);
            outputFileName = fileName + "_output.bin";
            fileName += ".bin";
            if (isExist(fileName)) main();
            break;
        } else if (choice == 'e' || choice == 'E') {
            cout << "Enter the name of the file without an extension:_";
            getline(cin, fileName);
            outputFileName = fileName + "_output.bin";
            fileName += ".bin";
            if(outputFile(fileName)) main();
            break;
        } else{
            cerr << "You should enter only E or N. Try again" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.ignore();
        }
    }
    inputFile(fileName);
    processFile(fileName, outputFileName);
    outputFile(fileName);
    outputFile(outputFileName);
}