/*
 * ElementaryBehavior.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: eloy
 */

#include "ElementaryBehavior.hpp"

void ElementaryBehavior::setUp()
{
	//Connect intention and CoS nodes
	mIntentionNode.setInputNode(&mCoSNode, -1);
	mCoSNode.setInputNode(&mIntentionNode, 0.5);

	//Connect the intention node to the intention field
	int IntNodeOutputNum = mIntentionNode.setOutput(mIntentionField.mMatSize);
	vector<vector<int>> dimPairs;
	for (int dimNum = 0; dimNum < mIntentionField.mDimensions; dimNum++)
		dimPairs.push_back({dimNum, dimNum});
	mIntentionField.setInput(mIntentionNode.mOutputs[IntNodeOutputNum-1], 1, dimPairs, nullptr);

	//Connect the CoS field to the CoS node
	mCoSNode.setInputField(&mCoSField, 1);
}
