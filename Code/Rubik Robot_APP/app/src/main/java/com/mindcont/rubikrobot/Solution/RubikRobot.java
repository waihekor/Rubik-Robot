package com.mindcont.rubikrobot.Solution;

import com.mindcont.rubikrobot.InstructionOptimization.TrinaryTree;

/**
  * Created by Administrator on 2016/2/26.
  **/
public class RubikRobot {

    public static Search search=new Search();
    TrinaryTree trinarytree =new TrinaryTree();//对魔方复原算法得到的复原指令进行优化解析

    //将魔方复原指令按照自定义协议包装，开始字符为'#',结束字符为'!'
    public static String Change_chars(String solution)
    {
        String solution2=solution.replace(" ","");
        StringBuffer chars=new StringBuffer(solution2);
        //chars.insert(0,"#");
        //chars.insert(chars.length(),"!");
        return (chars.toString());
    }

    /*
     *在给定的时间内给出Kociemba算法最优解
     */
    public String Get_Solution(String chars,int mil_time)
    {
        int maxDepth=21;
        String min_rubik_solution=new String();
        int min_step=10000;
        int step;
        long t = System.nanoTime();
        String rubik_solution=search.solution(chars, maxDepth, 100, 0, 0);

        while (rubik_solution.startsWith("Error 8") && ((System.nanoTime() - t) <2* 1.0e9))
        {
            rubik_solution = search.next(100, 0, 0);
        }

        if(rubik_solution.contains("Error"))
           return "#Error!";

        long t2 = System.nanoTime();
        while((System.nanoTime()-t2)<mil_time*10e6&&(!rubik_solution.contains("Error")))
        {
                step=trinarytree.getStepNum_Equation(rubik_solution);
                if (step<min_step)
                {
                    min_step=step;
                    min_rubik_solution = rubik_solution;
                }
            rubik_solution=search.solution(chars, 21, 100, 0, 0);
        }
        return (trinarytree.Pack_Optim_Equation(Change_chars(min_rubik_solution)));
    }

    public String Get_Solution(String chars)
    {
        String rubik_solution=search.solution(chars,22,100,0,0);
        return (trinarytree.Pack_Optim_Equation(Change_chars(rubik_solution)));
    }

    //测试是否满足魔方解算公式的各种条件
    public String Test_Solution(String chars)
    {
        String rubik_solution=search.solution(chars,22,100,0,0);
        return rubik_solution;
    }


}
