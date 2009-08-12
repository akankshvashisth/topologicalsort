#ifndef GRAPHNODE_HPP_INCLUDED
#define GRAPHNODE_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////
/* A GraphNode class that manages information about its parents and its
   children in order to allow higher managerial classes to implement 
   algorithms using the interface of the node */
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "utility.hpp"
//////////////////////////////////////////////////////////////////////////
class GraphNode : private boost::noncopyable
{
public:
    GraphNode(const std::string& aNode) :
            msName(aNode)
    { }
    const std::string& getName( void ) const;
    bool hasChildren( void ) const;
    bool hasParents( void ) const;
    unsigned getNumberOfParents( void ) const;
    unsigned getNumberOfChildren( void ) const;
    //
    // This function also adds "this" as a parent to the 
    // corresponding node aChild.
    //
    void addChild( const boost::shared_ptr<GraphNode>& aChild );
    const std::vector<GraphNode*>& getParentVec ( void ) const ;
    const std::vector< boost::shared_ptr<GraphNode> >& getChildrenVec ( void ) const;
    void printGraphNodeState ( void ) const;
    bool doesChildExist( const std::string& aChild ) const;
    bool doesParentExist( const std::string& aParent ) const;
    ~GraphNode();
private:
    void addParent( GraphNode* const &aParent );
    void removeParent ( const GraphNode* const aParent ) ;
    void removeChild ( const GraphNode* const aChild ) ;
    std::vector<GraphNode*>::const_iterator getNodeIterInParentVec( const std::string& aParent ) const;
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator getNodeIterInChildrenVec( const std::string& aChild ) const;
    std::string msName;
    std::vector< boost::shared_ptr<GraphNode> > mvChildren;
    std::vector<GraphNode*> mvParents;

    friend struct removeParentFunctor;
    friend struct removeChildFunctor;
};
//////////////////////////////////////////////////////////////////////////
#endif // GRAPHNODE_HPP_INCLUDED