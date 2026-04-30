#include "push_swap.h"
#include "unit.h"

static int is_stack_sorted(t_stack *stack)
{
    if (!stack)
        return (1);
    while (stack->next)
    {
        if (stack->value > stack->next->value)
            return (0);
        stack = stack->next;
    }
    return (1);
}

int test_cases()
{
    t_ctx ctx;
    ctx.stack = NULL;
    ctx.sort = NULL;
    ctx.bench = 0;

    // Single node test
    {
        t_stack *stack_a = NULL;
        t_stack *stack_b = NULL;
        
        stack_push(&stack_a, 42);
        simple_sort(&stack_a, &stack_b, &ctx);
        
        int value = stack_pop(&stack_a);
        stack_clear(&stack_a);
        
        TEST(value, 42, "Single node test failed: Value changed");
        TEST(stack_b == NULL, 1, "Single node test failed: Stack B used");
    }

    // Empty stack 
    {
        t_stack *stack_a = NULL;
        t_stack *stack_b = NULL;
    
        simple_sort(&stack_a, &stack_b, &ctx);
    
        TEST(stack_a == NULL, 1, "Empty stack test failed: stack_a changed");
        TEST(stack_b == NULL, 1, "Empty stack test failed: stack_b changed");
    }

    // Already sorted
	{
		t_stack *stack_a = NULL;
		t_stack *stack_b = NULL;

		stack_push(&stack_a, 3);
		stack_push(&stack_a, 2);
		stack_push(&stack_a, 1);

		simple_sort(&stack_a, &stack_b, &ctx);

		int n1 = stack_pop(&stack_a);
		int n2 = stack_pop(&stack_a);
		int n3 = stack_pop(&stack_a);

		stack_clear(&stack_a);
		stack_clear(&stack_b);

		TEST(n1, 1, TEXT("Already sorted failed. Expected top: 1, Was: %d", n1));
		TEST(n2, 2, TEXT("Already sorted failed. Expected middle: 2, Was: %d", n2));
		TEST(n3, 3, TEXT("Already sorted failed. Expected bottom: 3, Was: %d", n3));
	}

    // worst case
	{
		t_stack *stack_a = NULL;
		t_stack *stack_b = NULL;

		stack_push(&stack_a, 1);
		stack_push(&stack_a, 2);
		stack_push(&stack_a, 3);

		simple_sort(&stack_a, &stack_b, &ctx);

		int n1 = stack_pop(&stack_a);
		int n2 = stack_pop(&stack_a);
		int n3 = stack_pop(&stack_a);

		stack_clear(&stack_a);
		stack_clear(&stack_b);

		TEST(n1, 1, TEXT("Worst case failed. Expected top: 1, Was: %d", n1));
		TEST(n2, 2, TEXT("Worst case failed. Expected middle: 2, Was: %d", n2));
		TEST(n3, 3, TEXT("Worst case failed. Expected bottom: 3, Was: %d", n3));
	}

    // with onyl negative values
	{
		t_stack *stack_a = NULL;
		t_stack *stack_b = NULL;

		stack_push(&stack_a, 0);
		stack_push(&stack_a, -5);
		stack_push(&stack_a, 42);

		simple_sort(&stack_a, &stack_b, &ctx);

		int n1 = stack_pop(&stack_a);
		int n2 = stack_pop(&stack_a);
		int n3 = stack_pop(&stack_a);

		stack_clear(&stack_a);
		stack_clear(&stack_b);

		TEST(n1, -5, TEXT("Negative failed. Expected top: -5, Was: %d", n1));
		TEST(n2, 0, TEXT("Negative failed. Expected middle: 0, Was: %d", n2));
		TEST(n3, 42, TEXT("Negative failed. Expected bottom: 42, Was: %d", n3));
	}

    // with negative & positive values (mixed)
	{
		t_stack *stack_a = NULL;
		t_stack *stack_b = NULL;

		stack_push(&stack_a, 0);
		stack_push(&stack_a, -5);
		stack_push(&stack_a, 42);

		simple_sort(&stack_a, &stack_b, &ctx);

		int n1 = stack_pop(&stack_a);
		int n2 = stack_pop(&stack_a);
		int n3 = stack_pop(&stack_a);

		stack_clear(&stack_a);
		stack_clear(&stack_b);

		TEST(n1, -5, TEXT("Mixed failed. Expected top: -5, Was: %d", n1));
		TEST(n2, 0, TEXT("Mixed failed. Expected middle: 0, Was: %d", n2));
		TEST(n3, 42, TEXT("Mixed failed. Expected bottom: 42, Was: %d", n3));
	}

    // to ensure stack b is empty after applying sorting algorithm
	{
		t_stack *stack_a = NULL;
		t_stack *stack_b = NULL;
        int is_b_empty = 1;

		stack_push(&stack_a, 20);
		stack_push(&stack_a, 5);
		stack_push(&stack_a, 10);

		simple_sort(&stack_a, &stack_b, &ctx);
		
		if (stack_b != NULL)
            is_b_empty = 0;

		stack_clear(&stack_a);
		stack_clear(&stack_b);

		TEST(is_b_empty, 1, "Stack B was not empty (NULL) after simple_sort!");
	}

	return (1);
}

int test_simple_sort()
{
    t_ctx ctx;
    ctx.stack = NULL;
    ctx.sort = NULL;
    ctx.bench = 0;

    t_stack *stack_a = NULL;
    t_stack *stack_b = NULL;
    int i;

    i = 0;
    while (i < 100)
    {
        stack_push(&stack_a, i);
        i++;
    }

    simple_sort(&stack_a, &stack_b, &ctx);

    int is_sorted = is_stack_sorted(stack_a);
	stack_clear(&stack_a);
	stack_clear(&stack_b);
    TEST(is_sorted, 1, "Simple sort failed: 100 elements are not perfectly sorted");

    return (1);
}

REGISTER_TEST(test_cases);
REGISTER_TEST(test_simple_sort);
