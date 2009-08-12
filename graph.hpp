#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////
/* A Graph class to manage GraphNodes in order to topologically sort a
   Directed Acyclic Graph in order of dependency */
//////////////////////////////////////////////////////////////////////////
#include "graphnode.hpp"
//////////////////////////////////////////////////////////////////////////
class Graph : private boost::noncopyable
{
    public:
        Graph( void ) { /* Empty */ };
        //
        // If the node exists already then the function does nothing.
        //
        void addNode( const std::string& aNode );
        //
        // If node is not present, then function ignores the call.
        //
        unsigned getNumberOfNodes( void ) const ;
        //
        // If dependency already present, then the call is ignored.
        //
        void setDependency( const std::string& aParent, const std::string& aChild );
        //
        // Fills the argument vector (does not append but clears and adds) 
        // with the solution of the topological sort
        // This sort corrupts the Graph after being used, but allow for
        // information about cycles in dependency if any.
        //
        void topologicalSortRoot ( std::vector<std::string>& aNodeVec );
        //
        // Fills the argument vector with the solution of the
        // topological sort of the graph (does not append but clears and adds)
        //
        void topologicalSort ( std::vector<std::string>& aNodeVec ) const;
    private:
        void removeNode( const std::string& aNode );
        bool doesNodeExist( const std::string& aNode ) const ;
        void getLeafNodes( std::vector<std::string>& aNodeVec ) const ;
        void getRootNodes( std::vector<std::string>& aNodeVec ) const;
        bool isRootNode( const std::string& aNode ) const;
        bool isLeafNode( const std::string& aNode ) const ;
        void getParentsOf( const std::string& aNode, std::vector<std::string>& aNodeVec ) const;
        void getChildrenOf( const std::string& aNode, std::vector<std::string>& aNodeVec ) const;
        std::vector< boost::shared_ptr<GraphNode> >::const_iterator getNodeIterInGraph( const std::string& aNode ) const;
        std::vector< boost::shared_ptr<GraphNode> > mGraphNodes;

        friend struct pushbackAndRemoveNode;
        friend struct setTopSortData;
};
//////////////////////////////////////////////////////////////////////////
void appendUniqueNodeToNodeVec(const std::string& aNode, std::vector<std::string>& aNodeVec);
//////////////////////////////////////////////////////////////////////////
std::vector< std::pair<unsigned, boost::shared_ptr<GraphNode>> >::iterator 
getNodeIterInPairVec ( const std::string& aNode, 
                        std::vector< std::pair<unsigned, boost::shared_ptr<GraphNode>> >& aPairVec );
//////////////////////////////////////////////////////////////////////////
std::vector<std::string>::const_iterator getNodeIterInNodeVec( const std::string& aNode, const std::vector<std::string>& aNodeVec ); 
//////////////////////////////////////////////////////////////////////////
void appendNodeFrom( const std::string& aNode, std::vector<std::string>& aNodeVec );
void removeNodeFrom( const std::string& aNode, std::vector<std::string>& aNodeVec );
//////////////////////////////////////////////////////////////////////////
void addNodes( Graph& G, const std::vector<std::string>& aNodeVector );
void setDependencies ( Graph& G, const std::vector< std::pair< std::string, std::string> >& aNodePairVec );
//////////////////////////////////////////////////////////////////////////
#endif // GRAPH_HPP_INCLUDED
