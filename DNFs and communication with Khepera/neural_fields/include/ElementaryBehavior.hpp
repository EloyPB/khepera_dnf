/*
 * ElementaryBehavior.hpp
 *
 *  Created on: Dec 16, 2016
 *      Author: eloy
 */

#ifndef INCLUDE_ELEMENTARYBEHAVIOR_HPP_
#define INCLUDE_ELEMENTARYBEHAVIOR_HPP_

#include "NeuralField.hpp"
#include "NeuralNode.hpp"

class ElementaryBehavior
{
public:

	NeuralNode mIntentionNode;
	NeuralField mIntentionField;
	NeuralNode mCoSNode;
	NeuralField mCoSField;

	ElementaryBehavior(const char* pIntentionNodeConfig, const char* pIntentionFieldConfig,
			const char* pCoSNodeConfig, const char* pCoSFieldConfig) :
                mIntentionNode("intention node", pIntentionNodeConfig), mIntentionField("intention field", pIntentionFieldConfig),
                mCoSNode("CoS node", pCoSNodeConfig), mCoSField("CoS field", pCoSFieldConfig) {setUp();}

	void setUp();
	void addIntentionNodeInput();
	void addCoSFieldInput();

};


#endif /* INCLUDE_ELEMENTARYBEHAVIOR_HPP_ */
