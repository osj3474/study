read.csv(파일명)

nrow : 행개수

ncol : 열개수

str : col 타입

rename(new = origin)

mutate(new = expression)

filter(condition)

group_by(col) %>% summarise(n = n()) %>% arrange(desc(n))

sample_n(100) : 100개 샘플 추출 // 복원추출하려면, replace = TRUE

sample_frac(.3) : 30% 샘플 추출

cbind(A, B) : 무작정 열 합치기

rbind(A, B) : 무작정 행 합치기

merge(A, B, key = col, all(all.x, all.y) =T) : key 기준으로 열 합치기, 기본(Inner join), all(Outer join) all.x(Left join), all.y(Right join)

