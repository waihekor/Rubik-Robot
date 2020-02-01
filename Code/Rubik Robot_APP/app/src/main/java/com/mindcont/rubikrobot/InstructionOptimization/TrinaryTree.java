package com.mindcont.rubikrobot.InstructionOptimization;

public class TrinaryTree {
	
	
	String getMinSubTrinaryTree(String str_SubInput,int []len_branch)
	{
		if (str_SubInput.length()<=2)
		{
			len_branch[0] = 0;
			return "";
		}
		if (str_SubInput.charAt(0)=='U' || str_SubInput.charAt(0)=='D')
		{
			//int NNlen=0,LLlen=0,FFlen=0;
			int NNlen[]=new int [1];
			NNlen[0]=0;
			
			int LLlen[]=new int [1];
			LLlen[0]=0;

			int FFlen[]=new int [1];
			FFlen[0]=0;
			
			String NNstr,LLstr,FFstr;
			String tmpNNstr,tmpLLstr,tmpFFstr;
			//string subEquation = str_SubInput.substr(2);

			tmpNNstr = Turn_Equachange(str_SubInput,"NN",0);
			tmpLLstr = Turn_Equachange(str_SubInput,"LL",0);
			tmpFFstr = Turn_Equachange(str_SubInput,"FF",0);
			
			NNstr = getMinSubTrinaryTree(tmpNNstr.substring(2),NNlen);
			LLstr = getMinSubTrinaryTree(tmpLLstr.substring(2),LLlen);
			FFstr = getMinSubTrinaryTree(tmpFFstr.substring(2),FFlen);

			NNlen[0]+=16;
			LLlen[0]+=10;
			FFlen[0]+=10;

			if(NNlen[0]<=LLlen[0] && NNlen[0]<=FFlen[0])
			{
				len_branch[0] = NNlen[0];
				NNstr = "LL"+tmpLLstr.substring(0,2)+"RR"+NNstr;
				return NNstr;
			}
			else if(LLlen[0]<=NNlen[0] &&LLlen[0]<=FFlen[0])
			{
				len_branch[0] = LLlen[0];
				LLstr = "LL"+tmpLLstr.substring(0,2)+LLstr;
				return LLstr;
			}
			else
			{
				len_branch[0] = FFlen[0];
				FFstr = "FF"+tmpFFstr.substring(0,2)+FFstr;
				return FFstr;
			}
		}
		else
		{
			int step_num=0;
			if (Connect_Action(str_SubInput.charAt(0),str_SubInput.charAt(2)))
			{
				String nowStr = getMinSubTrinaryTree(str_SubInput.substring(4),len_branch);
				if(str_SubInput.charAt(1)=='2'||str_SubInput.charAt(3)=='2')
				{
					step_num=8;		
				}
				else
				{
					step_num=4;			
				}
				
				len_branch[0]= len_branch[0]+step_num;
				//len_branch[0]= len_branch[0]+4;
				nowStr = str_SubInput.substring(0,4)+nowStr;
				return nowStr;
			}
			String nowStr = getMinSubTrinaryTree(str_SubInput.substring(2),len_branch);
			if(str_SubInput.charAt(1)=='2')
			{
				step_num=8;			
			}
			else
			{
				step_num=4;			
			}
			len_branch[0] = len_branch[0]+step_num;
			//len_branch[0] = len_branch[0]+4;
			nowStr = str_SubInput.substring(0,2)+nowStr;
			return nowStr;
		}

	}

	
	String getMinTrinaryTree(String str_input) 
	{
		int len_branch[]=new int [1];
		len_branch[0]=0;
		str_input=Add_ClockwiseMark(str_input)+"!!";
		return getMinSubTrinaryTree(str_input,len_branch);
	}
	
	String getMinSubBinaryTree(String str_SubInput,int []len_branch)
	{
		if (str_SubInput.length()<=2)
		{
			len_branch[0] = 0;
			return "";
		}
		if (str_SubInput.charAt(0)=='U' || str_SubInput.charAt(0)=='D')
		{
			//int NNlen=0,LLlen=0,FFlen=0;
			
			int LLlen[]=new int [1];
			LLlen[0]=0;

			int FFlen[]=new int [1];
			FFlen[0]=0;
			
			String NNstr,LLstr,FFstr;
			String tmpNNstr,tmpLLstr,tmpFFstr;
			//string subEquation = str_SubInput.substr(2);

			//tmpNNstr = Turn_Equachange(str_SubInput,"NN",0);
			tmpLLstr = Turn_Equachange(str_SubInput,"LL",0);
			tmpFFstr = Turn_Equachange(str_SubInput,"FF",0);
			//NNstr = getMinSubTree(tmpNNstr.substr(2),NNlen);
			LLstr = getMinSubBinaryTree(tmpLLstr.substring(2),LLlen);
			FFstr = getMinSubBinaryTree(tmpFFstr.substring(2),FFlen);

			//NNlen+=16;
			LLlen[0]+=10;
			FFlen[0]+=10;

			/*if(NNlen<=LLlen && NNlen<=FFlen)
			{
				len_branch = NNlen;
				NNstr = "LL"+tmpLLstr.substr(0,2)+"RR"+NNstr;
				return NNstr;
			}
			else */if (/*LLlen<=NNlen &&*/ LLlen[0]<=FFlen[0])
			{
				len_branch[0] = LLlen[0];
				LLstr = "LL"+tmpLLstr.substring(0,2)+LLstr;
				return LLstr;
			}
			else
			{
				len_branch[0] = FFlen[0];
				FFstr = "FF"+tmpFFstr.substring(0,2)+FFstr;
				return FFstr;
			}
		}
		else
		{
			int step_num=0;
			if (Connect_Action(str_SubInput.charAt(0),str_SubInput.charAt(2)))
			{
				String nowStr = getMinSubBinaryTree(str_SubInput.substring(4),len_branch);
				if(str_SubInput.charAt(1)=='2'||str_SubInput.charAt(3)=='2')
				{
					step_num=8;		
				}
				else
				{
					step_num=4;			
				}
				
				len_branch[0]= len_branch[0]+step_num;
				//len_branch[0]= len_branch[0]+4;
				nowStr = str_SubInput.substring(0,4)+nowStr;
				return nowStr;
			}
			String nowStr = getMinSubBinaryTree(str_SubInput.substring(2),len_branch);
			if(str_SubInput.charAt(1)=='2')
			{
				step_num=8;			
			}
			else
			{
				step_num=4;			
			}
			len_branch[0] = len_branch[0]+step_num;
			//len_branch[0] = len_branch[0]+4;
			nowStr = str_SubInput.substring(0,2)+nowStr;
			return nowStr;
		}

	}

	String getMinBinaryTree(String str_input) 
	{
		int len_branch[]=new int [1];
		
		str_input=Add_ClockwiseMark(str_input)+"!!";
		return getMinSubBinaryTree(str_input,len_branch);
	}

	
	/*
	 * 计算优化后的指令的舵机执行步数
	 */
	public int getStepNum_Equation(String  optim_equation)
	{
		int step_num=0;

		optim_equation=optim_equation+"!!!";
		for(int i=0;i<optim_equation.length()-3;)
		{
			if((Connect_Action(optim_equation.charAt(i),optim_equation.charAt(i+2)))&&((optim_equation.charAt(i)!=optim_equation.charAt(i+1))&&(optim_equation.charAt(i+2)!=optim_equation.charAt(i+3))))//可以连续执行的两个动作组
			{
				
				if(optim_equation.charAt(i+1)=='2'||optim_equation.charAt(i+3)=='2')
				{
					step_num+=8;
				}
				else
				{
					step_num+=4;	
				}
				
				i+=4;
			}
			else//不能连续执行
			{
				
				if(optim_equation.charAt(i)==optim_equation.charAt(i+1))
				{
					step_num+=6;
				}
				else if(optim_equation.charAt(i+1)=='2')
				{
					step_num+=8;
				}
				else
				{
					step_num+=4;	
				}
				
				i+=2;
				
			}
			
		}
				
		return step_num;
		
	}
	
	
	/*
	 *对魔方解算算法生成的字符串，在原来顺时针单指令的后面加上一个字符'^'，便于下位机解析
	 *
	 */
	public String Add_ClockwiseMark(String equation)
	{
		equation=equation+"!";
		for(int i = 0;i<equation.length()-1;)
		{
			if((equation.charAt(i)<=0X5a&&equation.charAt(i)>=0X41)&&((equation.charAt(i+1)<=0X5a&&equation.charAt(i+1)>=0X41)||(equation.charAt(i+1)=='!')))
			{
				StringBuffer euqa_change=new StringBuffer(equation);
				euqa_change.insert(i+1, '^');
				equation=euqa_change.toString();
				i+=2;
			}
			else
			{
				i++;
			}
			
		}
		StringBuffer equation_add=new StringBuffer(equation);
		equation_add.deleteCharAt(equation_add.length()-1);
		equation=equation_add.toString();
		
		return equation;
	}
	
	
	/*魔方的位置在发生翻转时，得到变化之后的复原指令，有前后左右四种位置变化
	 *start_line是开始改变的字符串标号（包括start_line）
	 *change_sig有四种取值：L,R,F,B
	 */
	public  String Turn_Equachange(String equation,String change_sig,int start_line)
	{
		char L_sig,R_sig,F_sig,B_sig,U_sig,D_sig;
		String equation1=equation.substring(0,start_line); //不包括start_line
		String equation2=equation.substring(start_line);   //包括start_line
		StringBuffer equation3=new StringBuffer(equation2);
		char change_sign=change_sig.charAt(0);
		switch(change_sign)
		{
			case 'L':
			{
				L_sig='D';
				R_sig='U';
				F_sig='F';
				B_sig='B';
				U_sig='L';
				D_sig='R';

			}break;
			
			case 'R':
			{
				L_sig='U';
				R_sig='D';
				F_sig='F';
				B_sig='B';
				U_sig='R';
				D_sig='L';
				
			}break;
			
			
			case 'F':
			{
				L_sig='L';
				R_sig='R';
				F_sig='D';
				B_sig='U';
				U_sig='F';
				D_sig='B';
				
			}break;
			
			case 'B':
			{
				L_sig='L';
				R_sig='R';
				F_sig='U';
				B_sig='D';
				U_sig='B';
				D_sig='F';
				
			}break;
			default:
			{
				L_sig='L';
				R_sig='R';
				F_sig='F';
				B_sig='B';
				U_sig='U';
				D_sig='D';	
				
			}break;
			
		}
		
		for(int i=0;i<equation3.length();i++)
		{
				switch(equation3.charAt(i))
				{
					case 'L':
					{
						equation3.setCharAt(i,L_sig);
					}break;
					
					case 'R':
					{
						equation3.setCharAt(i,R_sig);						
					}break;
					
					case 'F':
					{
						equation3.setCharAt(i,F_sig);
					}break;
					
					case 'B':
					{
						equation3.setCharAt(i,B_sig);
					}break;
					
					case 'U':
					{
						equation3.setCharAt(i,U_sig);
					}break;
					
					case 'D':
					{
						equation3.setCharAt(i,D_sig);
					}break;
					
					default:
					{
						
					}break;
						
				}
		}

		return (equation1+equation3.toString());
	}
	

	
	public  boolean  Connect_Action( char char1,char char2)
	{	
		if((char1=='L'&&char2=='R')||(char1=='R'&&char2=='L')||(char1=='F'&&char2=='B')||(char1=='B'&&char2=='F'))
		{
			return true;
		}	
		
		else 
		{
			return false;	
		}		
	}
	
	/*
	 *为优化的魔方复原指令加上开始和结束标志位
	 */
	public String Pack_Optim_Equation(String equation )
	{
		StringBuffer pack=new StringBuffer(getMinBinaryTree(equation));
		pack.insert(0,'#');
		pack.insert(pack.length(),'!');
		return(pack.toString());
	}
	
	/*
	 * 把魔方解算算法解出来的复原公式转化为下位机能够识别的指令，但不做优化
	 * 备注：方便比较优化效果
	 * 
	 */
	public String No_Optim_Equation(String equation)
	{
		String change_equation="";
		equation=Add_ClockwiseMark(equation);
		for(int i=0;i<equation.length();)
		{
			if(equation.charAt(i)=='L'||equation.charAt(i)=='R'||equation.charAt(i)=='F'||equation.charAt(i)=='B')
			{
				change_equation=equation.substring(i,i+2)+change_equation;	
			}
			else
			{
				change_equation="LL"+Turn_Equachange(equation.substring(i,i+2),"LL",0)+"RR"+change_equation;
			}
			
			i+=2;
		}
		
		return change_equation; 
		
	}
	


	/*
	 * 计算未优化前的指令对应的舵机执行步数
	 */
	int getStepNum_Nooptim(String  nooptim_equation)
	{
		int step_num=0;
		nooptim_equation=No_Optim_Equation(nooptim_equation);
		nooptim_equation+="!";
		for(int i=0;i<nooptim_equation.length()-1;)
		{
			if(nooptim_equation.charAt(i)==nooptim_equation.charAt(i+1))
			{
				step_num+=6;
			}
			else if(nooptim_equation.charAt(i+1)=='2')
			{
				step_num+=8;	
			}
			else
			{
				step_num+=4;		
			}
			
			i+=2;
		}
				
		return step_num;
		
	}
}


