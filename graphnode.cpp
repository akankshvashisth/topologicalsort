#include "graphnode.hpp"
//////////////////////////////////////////////////////////////////////////
struct removeParentFunctor
{
    removeParentFunctor( const GraphNode* const aP ) : aParent(aP) {};
    void operator()( boost::shared_ptr<GraphNode>& aChild ) const
    {
        aChild->removeParent( aParent );
    }
    const GraphNode* const aParent;
};
//////////////////////////////////////////////////////////////////////////
struct removeChildFunctor
{
    removeChildFunctor( const GraphNode* const aC ) : aChild(aC) {};
    void operator()( GraphNode* &aParent ) const
    {
        aParent->removeChild( aChild );
    }
    const GraphNode* const aChild;
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct equalGraphNodeToNode
{
    equalGraphNodeToNode(const std::string& aNode):node(aNode){}
    bool operator()( const T& t ) const
    {
        return (t->getName() == node);
    }
    const std::string node;
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct coutNameFunctor
{
    void operator()( const T& aGNP )
    {
        std::cout << aGNP->getName() << ", ";
    }
};
//////////////////////////////////////////////////////////////////////////
GraphNode::~GraphNode()
{
    for_all( mvParents, removeChildFunctor(this));
    for_all( mvChildren, removeParentFunctor(this));
}
//////////////////////////////////////////////////////////////////////////
const std::string& GraphNode::getName( void ) const
{
    return msName;
}
//////////////////////////////////////////////////////////////////////////
bool GraphNode::hasChildren( void ) const
{
    return !(mvChildren.empty());
}
//////////////////////////////////////////////////////////////////////////
bool GraphNode::hasParents( void ) const
{
    return !(mvParents.empty());
}
//////////////////////////////////////////////////////////////////////////
unsigned GraphNode::getNumberOfParents( void ) const
{
    return static_cast<unsigned>(mvParents.size());
}
//////////////////////////////////////////////////////////////////////////
unsigned GraphNode::getNumberOfChildren( void ) const
{
    return static_cast<unsigned>(mvChildren.size());
}
//////////////////////////////////////////////////////////////////////////
void GraphNode::addParent( GraphNode* const &aParent )
{
    mvParents.push_back( aParent );
}
//////////////////////////////////////////////////////////////////////////
void GraphNode::addChild( const boost::shared_ptr<GraphNode>& aChild )
{
    mvChildren.push_back( aChild );
    aChild->addParent(this);
}
//////////////////////////////////////////////////////////////////////////
const std::vector<GraphNode*>& GraphNode::getParentVec ( void ) const
{
    return mvParents;
}
//////////////////////////////////////////////////////////////////////////
const std::vector< boost::shared_ptr<GraphNode> >& GraphNode::getChildrenVec ( void ) const
{
    return mvChildren;
}
//////////////////////////////////////////////////////////////////////////
void GraphNode::printGraphNodeState ( void ) const
{
    std::cout << "****************************" << std::endl;
    std::cout << "Name = " << msName << std::endl;
    if( hasParents() )
    {
        std::cout << "---- Parents ----" << std::endl;
        for_all( mvParents, coutNameFunctor<GraphNode*>() );
        std::cout << std::endl;
    }
    if( hasChildren() )
    {
        std::cout << "---- Children ----" << std::endl;
        for_all( mvChildren, coutNameFunctor<boost::shared_ptr<GraphNode>>() );
        std::cout << std::endl;
    }
    std::cout << "****************************" << std::endl;
}
//////////////////////////////////////////////////////////////////////////
void GraphNode::removeParent ( const GraphNode* const aParent ) 
{
    std::vector<GraphNode*>::iterator iter = find_if( mvParents.begin(), mvParents.end(), 
                                        equalGraphNodeToNode<GraphNode*>( aParent->getName() ));

    if ( iter != mvParents.end() )
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Removing Parent: " << aParent->getName()
            << " - from Child: " << msName << std::endl;
#endif
        mvParents.erase( iter );
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << aParent->getName() << " is not a parent of " 
            << msName << " - cannot remove" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void GraphNode::removeChild ( const GraphNode* const aChild ) 
{
    std::vector< boost::shared_ptr<GraphNode> >::iterator iter = find_if( mvChildren.begin(), mvChildren.end(), 
                                           equalGraphNodeToNode<boost::shared_ptr<GraphNode>>( aChild->getName() ));
    if ( iter != mvChildren.end() )
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Removing Child: " << aChild->getName()
            << " - from Parent: " << msName << std::endl;
#endif
        mvChildren.erase( iter );
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << aChild->getName() << " is not a child of " 
            << msName << " - cannot remove" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
std::vector<GraphNode*>::const_iterator GraphNode::getNodeIterInParentVec( const std::string& aParent ) const
{
    return find_if( mvParents.begin(), mvParents.end(), equalGraphNodeToNode<GraphNode*>(aParent));
}
//////////////////////////////////////////////////////////////////////////
std::vector< boost::shared_ptr<GraphNode> >::const_iterator GraphNode::getNodeIterInChildrenVec( const std::string& aChild ) const
{
    return find_if( mvChildren.begin(), mvChildren.end(), equalGraphNodeToNode<boost::shared_ptr<GraphNode>>(aChild) );
}
//////////////////////////////////////////////////////////////////////////
bool GraphNode::doesChildExist( const std::string& aChild ) const
{
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInChildrenVec( aChild );
    if ( iter != mvChildren.end())
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Child " << aChild << " exists in Parent " << this->getName() << std::endl;
#endif
        return true;
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Child " << aChild << " does not exist in Parent " << this->getName() << std::endl;
#endif
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////
bool GraphNode::doesParentExist( const std::string& aParent ) const
{
    std::vector<GraphNode*>::const_iterator iter = getNodeIterInParentVec( aParent );
    if ( iter != mvParents.end())
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Parent " << aParent << " exists in Child " << this->getName() << std::endl;
#endif
        return true;
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Parent " << aParent << " does not exist in Child " << this->getName() << std::endl;
#endif
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////
