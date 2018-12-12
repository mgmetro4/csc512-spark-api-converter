import org.apache.spark.sql.Encoder
import org.apache.spark.sql.Encoders
import org.apache.spark.sql.catalyst.encoders.ExpressionEncoder
import org.apache.spark.sql.expressions.Aggregator
def reduceAggregator[T](func: (T,T) => T)(implicit encoder: Encoder[T]) = {
    (new Aggregator[T, (Boolean, T), T] {
        def zero: (Boolean, T) = (false, null.asInstanceOf[T])
        def reduce(b: (Boolean, T), a: T): (Boolean, T) = {
            if (b._1) {
              (true, func(b._2, a))
            } else {
              (true, a)
            }
        }
        def merge(b1: (Boolean, T), b2: (Boolean, T)): (Boolean, T) = {
            if (!b1._1) {
              b2
            } else if (!b2._1) {
              b1
            } else {
              (true, func(b1._2, b2._2))
            }
        }
        def finish(reduction: (Boolean, T)): T = {
            if (!reduction._1) {
              throw new IllegalStateException("ReduceAggregator requires at least one input row")
            }
            reduction._2
        }
        def bufferEncoder: Encoder[(Boolean, T)] = Encoders.tuple(implicitly[Encoder[Boolean]], encoder)
        def outputEncoder: Encoder[T] = encoder
    }).toColumn
}

import org.apache.spark.sql.Encoder
import org.apache.spark.sql.Encoders
import org.apache.spark.sql.catalyst.encoders.ExpressionEncoder
import org.apache.spark.sql.expressions.Aggregator
def reduceByKeyAggregator[_, T](func: (T,T) => T)(implicit encoder: Encoder[T]) = {
    (new Aggregator[(_, T), (Boolean, T), T] {
        def zero: (Boolean, T) = (false, null.asInstanceOf[T])
        def reduce(b: (Boolean, T), a: (_, T)): (Boolean, T) = {
            if (b._1) {
              (true, func(b._2, a._2))
            } else {
              (true, a._2)
            }
        }
        def merge(b1: (Boolean, T), b2: (Boolean, T)): (Boolean, T) = {
            if (!b1._1) {
              b2
            } else if (!b2._1) {
              b1
            } else {
              (true, func(b1._2, b2._2))
            }
        }
        def finish(reduction: (Boolean, T)): T = {
            if (!reduction._1) {
              throw new IllegalStateException("ReduceAggregator requires at least one input row")
            }
            reduction._2
        }
        def bufferEncoder: Encoder[(Boolean, T)] = Encoders.tuple(implicitly[Encoder[Boolean]], encoder)
        def outputEncoder: Encoder[T] = encoder
    }).toColumn
}
