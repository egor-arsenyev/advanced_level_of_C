#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Константы
#define EPSILON_ROOT (double)1e-10
#define EPSILON_INTEGRAL (double)1e-10
#define INTERVAL_START (double)0
#define INTERVAL_END (double)10

typedef double (*func)(double);

// Определение функций
double f1(double x) { return (0.6 * x) + 3; }

double f2(double x) { return pow((x - 2), 3) - 1; }

double f3(double x) {
  if (fabs(x) < EPSILON_ROOT) { // Учитываем погрешность
    fprintf(stderr, "Ошибка: деление на ноль в f3(x).\n");
    exit(EXIT_FAILURE);
  }
  return (3 / x);
}

// Производные функций
double df1(double x) {
  (void)x; // Явно указываем, что параметр не используется
  return (double)0.6;
}

double df2(double x) { return (3 * pow((x - 2), 2)); }

double df3(double x) {
  if (fabs(x) < EPSILON_ROOT) { // Учитываем погрешность
    fprintf(stderr, "Ошибка: деление на ноль в df3(x).\n");
    exit(EXIT_FAILURE);
  }
  return (-3 / (x * x));
}

double root(double (*f)(double), double (*g)(double), double (*df)(double),
            double (*dg)(double), double a, double b, double eps,
            int *iterations) {
  if (a >= b) {
    fprintf(stderr, "Ошибка: некорректный интервал [a, b].\n");
    exit(EXIT_FAILURE);
  }

  // Проверка пересечения функций в интервале
  if ((f(a) - g(a)) * (f(b) - g(b)) > 0) {
    fprintf(stderr, "Ошибка: функции не пересекаются в интервале [a, b].\n");
    exit(EXIT_FAILURE);
  }

  double xl = a;
  double xr = b;
  *iterations = 0;

  while (fabs(xl - xr) > 2 * eps) {
    double fl = f(xl) - g(xl);
    double fr = f(xr) - g(xr);
    double dfl = df(xl) - dg(xl);
    double dfr = df(xr) - dg(xr);
    double ddfl =
        (df(xl) - dg(xl)) / (xr - xl); // Приближение второй производной
    double ddfr =
        (df(xr) - dg(xr)) / (xr - xl); // Приближение второй производной

    // Обновление xl
    if (fl * ddfl < 0) {
      xl = xl - (fl * (xl - xr)) / (fl - fr);
    } else {
      xl = xl - fl / dfl;
    }

    // Обновление xr
    if (fr * ddfr < 0) {
      xr = xr - (fr * (xr - xl)) / (fr - fl);
    } else {
      xr = xr - fr / dfr;
    }

    (*iterations)++;

    // Ограничение на количество итераций (защита от бесконечного цикла)
    if (*iterations > 10000) {
      fprintf(stderr, "Ошибка: превышено максимальное количество итераций.\n");
      exit(EXIT_FAILURE);
    }
  }

  return (xl + xr) / 2.0;
}

// Функция вычисления определенного интеграла методом трапеций
double integral(double (*f)(double), double a, double b, double eps) {
  if (a >= b) {
    double temp = a;
    a = b;
    b = temp;
  }

  int n = 1;
  double h = (b - a) / n;
  double integral_prev = 0.0;
  double integral_curr = (f(a) + f(b)) / 2.0 * h;

  while (fabs(integral_curr - integral_prev) > eps) {
    integral_prev = integral_curr;
    n *= 2;
    h = (b - a) / n;
    integral_curr = 0.0;

    for (int i = 0; i < n; i++) {
      integral_curr += (f(a + i * h) + f(a + (i + 1) * h)) / 2.0 * h;
    }

    // Проверка на переполнение
    if (n > 1e6) {
      fprintf(stderr, "Ошибка: превышено максимальное количество разбиений.\n");
      exit(EXIT_FAILURE);
    }
  }

  return integral_curr;
}

// Печать справки
void print_help() {
  printf("Опции:\n");
  printf("  --help            Показать это сообщение\n");
  printf("  --roots           Найти точки пересечения кривых\n");
  printf("  --iterations      Показать число итераций для нахождения корней\n");
  printf("  --test            Запустить тесты для функций root и integral\n");
}

// Тестирование функций
void run_tests() {
  printf("Тестирование функции root...\n");
  int iterations = 0;
  double root_result = root(f1, f2, df1, df2, INTERVAL_START, INTERVAL_END,
                            EPSILON_ROOT, &iterations);
  printf("Корень между f1 и f2: x = %.6f, итерации = %d\n", root_result,
         iterations);

  printf("Тестирование функции integral...\n");
  double integral_result =
      integral(f1, INTERVAL_START, INTERVAL_END, EPSILON_INTEGRAL);
  printf("Интеграл f1 на [%.0f, %.0f]: %.6f\n", INTERVAL_START, INTERVAL_END,
         integral_result);
}

int main(int argc, char *argv[]) {
  int iterations = 0;
  // Вычисление точек пересечения
  double x1 = root(f1, f2, df1, df2, INTERVAL_START, INTERVAL_END, EPSILON_ROOT,
                   &iterations);
  double x2 = root(f2, f3, df2, df3, EPSILON_ROOT, INTERVAL_END, EPSILON_ROOT,
                   &iterations);
  double x3 =
      root(f1, f3, df1, df3, EPSILON_ROOT, 1.0, EPSILON_ROOT, &iterations);

  // Вычисление площади
  double area1 = integral(f1, x1, x2, EPSILON_INTEGRAL) -
                 integral(f2, x1, x2, EPSILON_INTEGRAL);
  double area2 = integral(f2, x2, x3, EPSILON_INTEGRAL) -
                 integral(f3, x2, x3, EPSILON_INTEGRAL);
  double total_area = fabs(area1) + fabs(area2);

  if (argc < 2) {
    printf("\nПлощадь, образуемая кривыми: %.6f\n", total_area);
    printf("\nВызовите программу с ключем --help для получения списка доступных "
           "опций.\n");
      return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "--help") == 0) {
    print_help();
  } else if (strcmp(argv[1], "--roots") == 0) {
    printf("Точки пересечения: x1 = %.6f, x2 = %.6f, x3 = %.6f\n", x1, x2, x3);
    printf("Площадь, образуемая кривыми: %.6f\n", total_area);
  } else if (strcmp(argv[1], "--iterations") == 0) {
    int iterations = 0;
    root(f1, f2, df1, df2, INTERVAL_START, INTERVAL_END, EPSILON_ROOT,
         &iterations);
    printf("Итерации для f1 и f2: %d\n", iterations);
    root(f2, f3, df2, df3, EPSILON_ROOT, INTERVAL_END, EPSILON_ROOT,
         &iterations);
    printf("Итерации для f2 и f3: %d\n", iterations);
    root(f1, f3, df1, df3, EPSILON_ROOT, 1.0, EPSILON_ROOT, &iterations);
    printf("Итерации для f1 и f3: %d\n", iterations);
  } else if (strcmp(argv[1], "--test") == 0) {
    run_tests();
  } else {
    fprintf(stderr,
            "Ошибка: неизвестная опция. Используйте --help для справки.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
