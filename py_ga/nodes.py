import math 
import random

FULL_FILL = 0
RAND_FILL = 1
HALF_FILL = 2

class Node:
	def __init__(self,data=None):
		self.right= None
		self.left = None
		self.unique=None
		self.data = data		
	def setData(self,data):
		self.data = data


def newNode(fill_s,depth,data,is_bin,get_leaf,get_op):
	rootNode=Node(data)
	if(depth==1):
		if(is_bin(rootNode.data)):
			rootNode.left =Node(get_leaf())
			rootNode.right=Node(get_leaf())
		else:
			rootNode.unique=Node(get_leaf())
	else:
		if(fill_s==FULL_FILL):
			if(is_bin(rootNode.data)):
				rootNode.left  = newNode(FULL_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				rootNode.right = newNode(FULL_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				return rootNode
			else:
				rootNode.unique =newNode(FULL_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				return rootNode
		if(fill_s==RAND_FILL):
			if(is_bin(rootNode.data)):
				coin = random.randint(0,1)
				if(coin<1):
					rootNode.right = newNode(RAND_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				else:
					rootNode.right = Node(get_leaf())
				coin = random.randint(0,1)
				if(coin<1):
					rootNode.left = newNode(RAND_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				else:
					rootNode.left = Node(get_leaf())
				return rootNode				
			else:
				coin =random.randint(0,1)
				if(coin<1):
					rootNode.unique = newNode(RAND_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				else:
					rootNode.unique = Node(get_leaf())
				return rootNode				
		if(fill_s==HALF_FILL):
			if(is_bin(rootNode.data)):
				rootNode.left = newNode(FULL_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				coin = random.randint(0,1)
				if(coin<1):
					rootNode.right = newNode(RAND_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				else:
					rootNode.right = Node(get_leaf())
				return rootNode
			else:
				rootNode.unique = newNode(HALF_FILL,depth-1,get_op(),is_bin,get_leaf,get_op)
				return rootNode
	return rootNode

def evalFunciton(rootNode, ExeBin, ExeUn, is_nd,is_bin, is_vr, gt_vr_i, gt_tr, gt_ex_i, vals):
	left_operand  =0
	right_operand =0
	unique_operand=0
	if(is_bin(rootNode.data)):
		if(is_nd(rootNode.left.data)):
			left_operand=evalFunciton(rootNode.left,ExeBin,ExeUn,is_nd,is_bin,is_vr,gt_vr_i,gt_tr,gt_ex_i,vals)
		else:
			if(is_vr(rootNode.left.data)):
				left_operand=vals[gt_vr_i(rootNode.left.data)]
			else:
				left_operand=gt_tr(rootNode.left.data)

		if(is_nd(rootNode.right.data)):
			right_operand=evalFunciton(rootNode.right,ExeBin,ExeUn,is_nd,is_bin,is_vr,gt_vr_i,gt_tr,gt_ex_i,vals)
		else:
			if(is_vr(rootNode.right.data)):
				right_operand=vals[gt_vr_i(rootNode.right.data)]
			else:
				right_operand=gt_tr(rootNode.right.data)
		return ExeBin(gt_ex_i(rootNode.data),left_operand,right_operand)
	else:
		if(is_nd(rootNode.unique.data)):
			unique_operand=evalFunciton(rootNode.unique,ExeBin,ExeUn,is_nd,is_bin,is_vr,gt_vr_i,gt_tr,gt_ex_i,vals)
		else:
			if(is_vr(rootNode.unique.data)):
				unique_operand=vals[gt_vr_i(rootNode.unique.data)]
			else:
				unique_operand=gt_tr(rootNode.unique.data)
		return ExeUn(gt_ex_i(rootNode.data),unique_operand)

def cuadraticError(rootNode,ExeBin,ExeUn,is_nd,is_bin,is_vr,gt_vr_i,gt_tr,gt_ex_i,l_values):
	error = 0
	var_len = len(l_values[0])
	for index in range(len(l_values)):
		error+=math.pow(l_values[index][var_len-1]-evalFunciton(rootNode,ExeBin,ExeUn,is_nd,is_bin,is_vr,gt_vr_i,gt_tr,gt_ex_i,l_values[index]),2)
	error = math.sqrt(error/len(l_values))
	return error

def nodeCount(rootNode,is_nd,is_bin):
	counter = 1 
	if(is_bin(rootNode.data)):
		if(rootNode.left!=None and is_nd(rootNode.left.data)):
			counter += nodeCount(rootNode.left,is_nd,is_bin)
		if(rootNode.right!=None and is_nd(rootNode.right.data)):
			counter += nodeCount(rootNode.right,is_nd,is_bin)
	else:
		if(rootNode.unique!=None and is_nd(rootNode.unique.data)):
			counter += nodeCount(rootNode.unique,is_nd,is_bin)
	return counter

def totalNodeCount(rootNode,is_bin):
	counter = 1 
	if(is_bin(rootNode.data)):
		if(rootNode.left!=None):
			counter += totalNodeCount(rootNode.left,is_bin)
		if(rootNode.right!=None):
			counter += totalNodeCount(rootNode.right,is_bin)
	else:
		if(rootNode.unique!=None):
			counter += totalNodeCount(rootNode.unique,is_bin)
	return counter

def printPosOrd(rootNode,is_nd,is_bin,gt_chr_rp):
	if(is_bin(rootNode.data)):
		print('( ',gt_chr_rp(rootNode.data),' ')
		if(rootNode.left!=None):
			if(is_nd(rootNode.left.data)):
				printPosOrd(rootNode.left,is_nd,is_bin,gt_chr_rp)
			else:
				print(gt_chr_rp(rootNode.left.data),' ')
		if(rootNode.right!=None):
			if(is_nd(rootNode.right.data)):
				printPosOrd(rootNode.right,is_nd,is_bin,gt_chr_rp)
			else:
				print(gt_chr_rp(rootNode.right.data),' ')
	else:
		print(gt_chr_rp(rootNode.data),'( ')
		if(rootNode.unique!=None):
			if(is_nd(rootNode.unique.data)):
				printPosOrd(rootNode.unique,is_nd,is_bin,gt_chr_rp)
			else:
				print(gt_chr_rp(rootNode.unique.data),' ')
		print(' )')

def getIST(index,rootNode,outNode,is_nd,is_bin):
	deep = index
	if(index==0):
		outNode=rootNode
	else:
		if(index>=1):
			if(is_bin(rootNode.data)):
				if(rootNode.left!=None and is_nd(rootNode.left.data)):
					deep=getIST(deep-1,rootNode.left,outNode,is_nd,is_bin)
				if(rootNode.right!=None and is_nd(rootNode.right.data)):
					deep=getIST(deep-1,rootNode.right,outNode,is_nd,is_bin)
			else:
				if(rootNode.unique!=None and is_nd(rootNode.unique.data)):
					deep=getIST(deep-1,rootNode.unique,outNode,is_nd,is_bin)
	return deep

def getIN(index,rootNode,outNode,is_nd,is_bin):
	deep = index
	if(index==0):
		outNode=rootNode
	else:
		if(index>=1):
			if(is_bin(rootNode.data)):
				if(rootNode.left!=None):
					deep=getIN(deep-1,rootNode.left,outNode,is_nd,is_bin)
				if(rootNode.right!=None):
					deep=getIN(deep-1,rootNode.right,outNode,is_nd,is_bin)
			else:
				if(rootNode.unique!=None):
					deep=getIN(deep-1,rootNode.unique,outNode,is_nd,is_bin)
	return deep

def setIST(index,rootNode,inNode,is_nd,is_bin):
	deep = index
	if(index==0):
		rootNode=inNode
	else:
		if(index>=1):
			if(is_bin(rootNode.data)):
				if(rootNode.left!=None and is_nd(rootNode.left.data)):
					deep=getIST(deep-1,rootNode.left,inNode,is_nd,is_bin)
				if(rootNode.right!=None and is_nd(rootNode.right.data)):
					deep=getIST(deep-1,rootNode.right,inNode,is_nd,is_bin)
			else:
				if(rootNode.unique!=None and is_nd(rootNode.unique.data)):
					deep=getIST(deep-1,rootNode.unique,inNode,is_nd,is_bin)
	return deep

def setIN(index,rootNode,inNode,is_nd,is_bin):
	deep = index
	if(index==0):
		rootNode=inNode
	else:
		if(index>=1):
			if(is_bin(rootNode.data)):
				if(rootNode.left!=None):
					deep=setIN(deep-1,rootNode.left,inNode,is_nd,is_bin)
				if(rootNode.right!=None):
					deep=setIN(deep-1,rootNode.right,inNode,is_nd,is_bin)
			else:
				if(rootNode.unique!=None):
					deep=setIN(deep-1,rootNode.unique,inNode,is_nd,is_bin)
	return deep