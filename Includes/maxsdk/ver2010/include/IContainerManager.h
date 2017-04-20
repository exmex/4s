/*****************************************************************************

	FILE: IContainerManager.h

	DESCRIPTION: Describes the interface for global container functionality.

	CREATED BY:	Nicolas Léonard

	HISTORY:	July 9th, 2008	Creation

 	Copyright (c) 2008, All Rights Reserved.
 *****************************************************************************/

#ifndef __ICONTAINER_MANAGER__H
#define __ICONTAINER_MANAGER__H

#include "iFnPub.h"


//! \brief The interface for global container functionality.
/*! Operations on a specific container can be accessed through the container itself. 
\see IContainerObject
*/
class IContainerManager : public FPStaticInterface
{
public:
	//! \brief Create a container
	/*! \return the container node if successfully created */
	/*! \remarks If created, the container node will be selected */
	/*! \remarks The container node will be created at the average of the content nodes, or at the origin if the Tab is empty */
	/*! \param[in] contentNodes - Nodes to be added as content of the container */
	virtual INode* CreateContainer(INodeTab& contentNodes) = 0;

	//! \brief Inherit a container into the scene
	/*! \return the container node if a container was successfully inherited */
	/*! \param[in] asset - The container asset representing the definition file to be used */
	/*! \remarks If the asset is invalid, the user will be presented with a file dialog. */
	virtual INode* CreateInheritedContainer(const MaxSDK::AssetManagement::AssetUser& asset) = 0;

	//! \brief Test if a node is in a container or not
	/*! \param[in] node - The node that we are testing to see if it is in a container */
	/*! \return the container interface if the node is in a container */
	virtual IContainerObject* IsInContainer(INode *node) = 0;

	//! \brief Test if a node is a container or not
	/*! \param[in] node - The node that we are testing to see if it is a container */
	/*! \return the container interface if the node is a container */
	virtual IContainerObject* IsContainerNode(INode *node) = 0;

	// Function IDs for function publishing
	enum 
	{ 
		IMPORT_CONTAINER_ID,
		CREATE_CONTAINER_ID,
		IS_IN_CONTAINER_ID,
		IS_CONTAINER_NODE_ID,
	};
};

//! \brief IContainerManager interface ID
#define IID_IContainerManager Interface_ID(0xD951AEE9, 0x5769E48B)

//! \brief Returns the container manager interface 
inline IContainerManager* GetContainerManagerInterface() { 
	return static_cast<IContainerManager*>(GetCOREInterface(IID_IContainerManager)); 
}


#endif __ICONTAINER_MANAGER__H
