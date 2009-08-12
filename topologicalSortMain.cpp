#include <fstream>
#include "graph.hpp"
#include "inputToGraph.hpp"
#include "topologicalSort_test_suite.hpp"
#include <HiResTime/hiResTime.hpp>
#include <aksUtilityHeaders/print.hpp>

using namespace std;

vector<boost::shared_ptr< aks::time::HighResStopWatch> > vCLOCK;


void RunTests( void )
{
    GraphNodeSetUp_test();
    GraphNodeDependencySetup_test();
    add_node_test();
    addNodes_test();
    set_dependency_test();
    setDependencies_test();
    appendNodefrom_removeNodefrom_test();
    topologySortRoot_test();
    topologicalSort_test();
}

struct coutContainer  
{
    coutContainer( void ) : i ( 1 ) { /*Empty*/ }
    void operator()( const string& aString )
    {
        cout << i << "\t: " << aString << endl;
        ++i;
    }
    unsigned i;
};

void topologicalSort( const std::string& graphSetupData )
{
    Graph G;
    consoleToGraph( G, graphSetupData );
    vector<string> solutionVector;
    (*(vCLOCK.begin()))->startTimer();
    G.topologicalSort( solutionVector );
    (* ( *( vCLOCK.begin() ) ) ).stopTimer();
    for_all( solutionVector, coutContainer() );
}

int main( int argc, char* argv[] )
{   
    vCLOCK.push_back( boost::shared_ptr<aks::time::HighResStopWatch>( new aks::time::HighResStopWatch ) );
    if ( argc > 1 )
    {
        string inputFlag( argv[1] );
        unsigned flag( 0 );
        if      ( inputFlag == "-help" )     flag = 0;
        else if ( inputFlag == "-console" )  flag = 1;
        else if ( inputFlag == "-file" )     flag = 2;
        else if ( inputFlag == "-test" )     flag = 3;
        else cout << "Input error \"- help\" for more information" << endl;

        flag = 2;
        switch ( flag )
        {
        case 0:
            inputToGraphHelp();
            break;
        case 1:
            if (argc > 2)
            {
                string graphSetup( argv[2] );
                topologicalSort( graphSetup );
            } 
            else
                cout << "Not enough parameters" << endl;
        	break;
        case 2:
            if (argc > 2)
            {
                string filename( argv[2] );
                filename = "topologicalSort.txt";
                ifstream filedata( filename.c_str() );
                if ( filedata )
                {
                    string graphSetup( (istreambuf_iterator<char>(filedata) ), 
                        istreambuf_iterator<char>() );
                    topologicalSort( graphSetup );
                } 
                else
                    cout << "Input file read error" << endl;
            }
            else
                cout << "Not enough parameters" << endl;
            break;
        case 3:
            RunTests();
        }
    }
    else
        cout << "Type \"-help\" for more information" << endl;

    aks::print::print( "Elapsed time :" , (*(*(vCLOCK.begin()))).getElapsedTime() );
    aks::print::print( "Total time   :" , (*(*(vCLOCK.begin()))).getTotalTimeElapsedSinceInitialization() );
    
    return EXIT_SUCCESS;
}
