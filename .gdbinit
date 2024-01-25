python

import math
import re

import gdb


class Core23Object:
    def __init__(self, forClass: str, this: gdb.Value):
        self.this: gdb.Value = this
        self.Type: str = forClass
        self.DisplayString: str = "{...}"
        self.name: str = forClass

    def to_string(self):
        return self.DisplayString

    def display_hint(self):
        return self.Type


class Core23PrettyPrinter(gdb.printing.PrettyPrinter):
    def __init__(self):
        super().__init__("Core23", None)
        self.printers: list[Core23PrettyPrinter.PrettyPrinter] = []
        self.name = "Core23 (GDB Pretty Printer)"
        self.enable = True
        self.views: dict[str, Core23PrettyPrinter.PrettyPrinter] = {}
        self.pattern = re.compile("^([a-zA-Z0-9_:]+)(<.*>)?$")

    def addPrinter(self, cls: str, printer: type):
        if self.pattern.match(cls) and cls not in self.views:
            prettyPrinter = Core23PrettyPrinter.PrettyPrinter(cls, printer)
            self.views[cls] = prettyPrinter
            self.printers.append(prettyPrinter)

        if cls in self.views and self.views[cls] != printer:
            i = self.printers.index(self.views[cls])
            self.printers[i] = printer
            self.views[cls] = print
        self.subprinters = self.printers

    def __call__(self, this: gdb.Value):
        class0 = this.type
        if class0.code == gdb.TYPE_CODE_REF:
            class0 = this.type.target()
            if hasattr(this, "referenced_value"):
                this = this.referenced_value()
        class0 = class0.unqualified().strip_typedefs()
        typename = class0.tag
        if typename is None:
            return None
        if this.dynamic_type is not None:
            trueType = this.dynamic_type.name
            if trueType != typename:
                # print("Use True type [%s] " % trueType)
                if trueType in self.views:
                    return self.views[trueType].print(this.dynamic_cast(this.dynamic_type.reference()))
                matcher = self.pattern.match(trueType)
                if matcher is not None:
                    classname = matcher.group(1)
                    if classname in self.views:
                        print("-- Updating Printer [%s] --" % trueType)
                        self.addPrinter(trueType, self.views[classname].printer)
                        return self.views[trueType].print(this)
        if typename in self.views:
            return self.views[typename].print(this)
        matcher = self.pattern.match(typename)
        if matcher is None:
            return None
        classname = matcher.group(1)
        if classname in self.views:
            print("-- Updating Printer [%s] --" % typename)
            self.addPrinter(typename, self.views[classname].printer)
            return self.views[typename].print(this)

    class PrettyPrinter(gdb.printing.SubPrettyPrinter):
        def __init__(self, cls: str, printer: type):
            super().__init__(cls)
            self.cls = cls
            self.printer = printer
            self.name = cls
            self.enable = True

        def print(self, this: gdb.Value):
            if this.type.code == gdb.TYPE_CODE_REF:
                if hasattr(this, "referenced_value"):
                    this = this.referenced_value()
            if self.cls != this.type.name:
                self.printer(self.cls, this)
                if this.type.name in Core23.views:
                    return Core23.views[this.type.name].printer(this.type.name, this)
            return self.printer(self.cls, this)

        def __str__(self) -> str:
            return f"Pretty-Printer: [Class={self.cls}; Printer={self.printer}]";


Core23 = Core23PrettyPrinter()


def registerPrinters():
    global Core23
    objFile = gdb.current_objfile()
    if objFile is not None:
        objFile.pretty_printers.append(Core23)
    else:
        gdb.pretty_printers.append(Core23)


def newPrinter(forClass, printer):
    global Core23
    Core23.addPrinter(forClass, printer)


# ================ [ Pretty Printers ] =============================

class Core23ExpendableObject(Core23Object):
    def __init__(self, forClass: str, this: gdb.Value):
        super().__init__(forClass, this)
        self.Size: int = -1
        self.ExpandItems: list[gdb.Value | str] = []

    def to_string(self):
        if self.DisplayString != "{...}":
            return self.DisplayString
        if self.Size >= 0:
            return "Size=" + str(self.Size)
        if len(self.ExpandItems) == 0:
            return "{}"
        formattedString = "{"
        if not isinstance(self.ExpandItems[0], tuple):
            i = 0
            for Item in self.ExpandItems:
                formattedString += "[%d]=%s".format(i, Item)
                i += 1
                if i < len(self.ExpandItems):
                    formattedString += ", "
                if len(formattedString) > 255:
                    formattedString += "..."
                    break
        else:
            i = 0
            for Item in self.ExpandItems:
                formattedString += "%s=%s".format(Item[0], Item[1])
                if i < len(self.ExpandItems):
                    formattedString += ", "
                if len(formattedString) > 255:
                    formattedString += "..."
                    break
        formattedString += "}"

    def children(self):
        if self.Size < 0:
            return Core23ExpendableObject.Expandable(self.ExpandItems, len(self.ExpandItems))
        else:
            return Core23ExpendableObject.Expandable(self.ExpandItems, self.Size)

    class Expandable:
        def __init__(self, Items, Size):
            self.Items = Items
            self.Size = Size
            self.i = 0
            self.isIndexed = (Size > 0) and not isinstance(Items[0], tuple)

        def __iter__(self):
            return self

        def __next__(self):
            if self.i == self.Size:
                raise StopIteration(self.i)
            current = self.Items[self.i]
            cursor = self.i
            self.i = cursor + 1
            if self.isIndexed:
                return ("%d" % cursor, current)
            else:
                return current


# Boolean, Byte, Short, Character, Integer, Long, Float, Double, Enum<E>, ...

class Core23ValueFieldObject(Core23Object):

    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        if forClass.endswith("AtomicReference"):
            if this['value'] == 0:
                self.DisplayString = "null"
            else:
                self.DisplayString = str(this["value"][0])
        else:
            self.DisplayString = str(this["value"]).split("::")[-1]


class Core23Throwable(Core23Object):

    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        self.DisplayString = str(this["msg"])


class Core23Complex(Core23Object):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        imag = this['ivalue']
        if math.isnan(imag) or (math.isinf(imag) and imag > 0):
            self.DisplayString = "%f+(%f)".format(float(this['rvalue']), float(this['ivalue']))
        elif math.isnan(imag) or (math.isinf(imag) and imag < 0):
            self.DisplayString = "%f-(%f)".format(float(this['rvalue']), -float(this['ivalue']))
        elif imag > 0:
            self.DisplayString = "%f+%f".format(float(this['rvalue']), float(this['ivalue']))
        else:
            self.DisplayString = "%f-%f".format(float(this['rvalue']), -float(this['ivalue']))


class Core23Trace(Core23ExpendableObject):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        self.ExpandItems = [
            ("File", this['fName']),
            ("Line", this['fLine']),
            ("Module", this['modName']),
            ("Class", this['clsName']),
            ("Method", this['metName']),
        ]


class Core23String(Core23Object):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        size = int(this['len'])
        chars = this['value'].cast(gdb.lookup_type("char16_t*"))
        self.DisplayString = chars.lazy_string(length=size)


class Core23ValueFieldArray(Core23ExpendableObject):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        IndexedItems = []
        self.Size = int(this['len'])
        for i in range(self.Size):
            if forClass.startswith("core::util::Atomic"):
                Item = this["array"]['value'][i]
            else:
                Item = this['value'][i]
            IndexedItems.append(Item)
        self.ExpandItems = IndexedItems


class Core23RefFieldArray(Core23ExpendableObject):
    def __init__(self, forClass, this: gdb.Value):
        super().__init__(forClass, this)
        IndexedItems = []
        self.Size = int(this['len'])
        for i in range(self.Size):
            if forClass.startswith("core::util::Atomic"):
                Item = this["array"]['value'][i]
            elif forClass.startswith("core::util::Array"):
                ElementType = this.type.template_argument(0).pointer()
                Item = this["data"]['value'][i]
                Item = Item.cast(ElementType)
            else:
                Item = this['value'][i]
            if Item != 0:
                IndexedItems.append(Item[0])
            else:
                IndexedItems.append("null")


class Core23ArrayList(Core23ExpendableObject):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        IndexedItems = []
        array = 0
        offset = 0
        try:
            # ArrayList<?>::SubList
            array = this['root']['array']
            self.Size = this['len']
            offset = this['offset']
        except gdb.error:
            try:
                # ArrayList<?>
                array = this['array']
                self.Size = this['len']
                offset = 0
            except gdb.error:
                # ArrayList<?>::ListItr
                newPrinter(this.type, Core23Object)
        finally:
            if self.Size < 0:
                Core23.views[forClass] = Core23PrettyPrinter.PrettyPrinter(forClass, Core23Object)
                return
            for i in range(self.Size):
                Item = array[i + offset][0]
                IndexedItems.append(Item)
            self.ExpandItems = IndexedItems


class Core23LinkedList(Core23ExpendableObject):
    def __init__(self, forClass: str, this: gdb.Value):
        super().__init__(forClass, this)
        IndexedItems = []
        try:
            # LinkedList<?>SubList
            self.Size = this['len']
        except gdb.error:
            ...
        finally:
            ...


class Core23PriorityQueue(Core23ExpendableObject):
    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        IndexedItems = []
        queue = 0
        try:
            # PriorityQueue<?>
            queue = this['queue']
            self.Size = this['len']
        except gdb.error:
            # PriorityQueue<?>::Itr
            self.Size = -1
        finally:
            if self.Size < 0:
                Core23.views[forClass] = Core23PrettyPrinter.PrettyPrinter(forClass, Core23Object)
                return
            for i in range(self.Size):
                Item = queue[i][0]
                IndexedItems.append(Item)
            self.ExpandItems = IndexedItems


class Core23LocalTime(Core23Object):

    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        if forClass != '' and not forClass.endswith('LocalTime'):
            return
        hhmmssSSSSSSSSS = this['hhmmssSSSSSSSSS']
        hh = (hhmmssSSSSSSSSS >> 42) & 0x1F
        mm = (hhmmssSSSSSSSSS >> 36) & 0x3F
        ss = (hhmmssSSSSSSSSS >> 30) & 0x3F
        SS = hhmmssSSSSSSSSS & 0x3FFFFFFF
        if SS == 0:
            if ss == 0:
                self.DisplayString = "%.2d:%.2d" % (hh, mm)
            else:
                self.DisplayString = "%.2d:%.2d:%.2d" % (hh, mm, ss)
        elif SS % 1000000 == 0:
            self.DisplayString = "%.2d:%.2d:%.2d.%.3d" % (hh, mm, ss, SS / 1000000)
        elif SS % 1000 == 0:
            self.DisplayString = "%.2d:%.2d:%.2d.%.6d" % (hh, mm, ss, SS / 1000)
        else:  # SS < 1000000000
            self.DisplayString = "%.2d:%.2d:%.2d.%.9d" % (hh, mm, ss, SS)


class Core23LocalDate(Core23Object):

    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        if forClass != '' and not forClass.endswith('LocalDate'):
            return
        yyyyMMdd = this['yyyyMMdd']
        sign = (yyyyMMdd >> 39) & 1
        yyyy = (yyyyMMdd >> 9) & 0x3FFFFFFF
        MM = (yyyyMMdd >> 5) & 0xF
        dd = yyyyMMdd & 0x1F
        if sign == 0:
            if yyyy < 10000:
                self.DisplayString = "%.4d/%.2d/%.2d" % (yyyy, MM, dd)
            else:
                self.DisplayString = "+%.4d/%.2d/%.2d" % (yyyy, MM, dd)
        else:
            self.DisplayString = "%-.4d/%.2d/%.2d" % (yyyy, MM, dd)


class Core23LocalDateTime(Core23LocalTime):

    def __init__(self, forClass, this):
        super().__init__(forClass, this)
        if forClass != '' and not forClass.endswith('LocalDateTime'):
            return
        yyyyMMdd = this['date']['yyyyMMdd']
        sign = (yyyyMMdd >> 39) & 1
        yyyy = (yyyyMMdd >> 9) & 0x3FFFFFFF
        MM = (yyyyMMdd >> 5) & 0xF
        dd = yyyyMMdd & 0x1F
        if sign == 0:
            if yyyy < 10000:
                self.DisplayString = "%.4d/%.2d/%.2d" % (yyyy, MM, dd)
            else:
                self.DisplayString = "+%.4d/%.2d/%.2d" % (yyyy, MM, dd)
        else:
            self.DisplayString = "%-.4d/%.2d/%.2d" % (yyyy, MM, dd)

        hhmmssSSSSSSSSS = this['time']['hhmmssSSSSSSSSS']
        hh = (hhmmssSSSSSSSSS >> 42) & 0x1F
        mm = (hhmmssSSSSSSSSS >> 36) & 0x3F
        ss = (hhmmssSSSSSSSSS >> 30) & 0x3F
        SS = hhmmssSSSSSSSSS & 0x3FFFFFFF
        if SS == 0:
            if ss == 0:
                self.DisplayString += " at %.2d:%.2d" % (hh, mm)
            else:
                self.DisplayString += " at %.2d:%.2d:%.2d" % (hh, mm, ss)
        elif SS % 1000000 == 0:
            self.DisplayString += " at %.2d:%.2d:%.2d.%.3d" % (hh, mm, ss, SS / 1000000)
        elif SS % 1000 == 0:
            self.DisplayString += " at %.2d:%.2d:%.2d.%.6d" % (hh, mm, ss, SS / 1000)
        else:  # SS < 1000000000
            self.DisplayString += " at %.2d:%.2d:%.2d.%.9d" % (hh, mm, ss, SS)


def registerPrettyPrinters():
    global Core23
    if Core23 is None:
        Core23 = Core23PrettyPrinter()

    newPrinter("void*", Core23Object)

    newPrinter("core::Object", Core23Object)
    newPrinter("core::util::Collection", Core23Object)
    newPrinter("core::util::List", Core23Object)
    newPrinter("core::util::Queue", Core23Object)
    newPrinter("core::util::Set", Core23Object)
    newPrinter("core::util::Map", Core23Object)
    newPrinter("core::private::Unsafe", Core23Object)

    newPrinter("core::Boolean", Core23ValueFieldObject)
    newPrinter("core::Byte", Core23ValueFieldObject)
    newPrinter("core::Short", Core23ValueFieldObject)
    newPrinter("core::Character", Core23ValueFieldObject)
    newPrinter("core::Integer", Core23ValueFieldObject)
    newPrinter("core::Long", Core23ValueFieldObject)
    newPrinter("core::Float", Core23ValueFieldObject)
    newPrinter("core::Double", Core23ValueFieldObject)
    newPrinter("core::Enum", Core23ValueFieldObject)
    newPrinter("core::util::AtomicInteger", Core23ValueFieldObject)
    newPrinter("core::util::AtomicBoolean", Core23ValueFieldObject)
    newPrinter("core::util::AtomicLong", Core23ValueFieldObject)
    newPrinter("core::util::AtomicReference", Core23ValueFieldObject)

    newPrinter("core::Throwable", Core23Throwable)
    newPrinter("core::Exception", Core23Throwable)
    newPrinter("core::Complex", Core23Complex)

    newPrinter("core::private::BooleanArray", Core23ValueFieldArray)
    newPrinter("core::private::ByteArray", Core23ValueFieldArray)
    newPrinter("core::private::ShortArray", Core23ValueFieldArray)
    newPrinter("core::private::CharArray", Core23ValueFieldArray)
    newPrinter("core::private::IntArray", Core23ValueFieldArray)
    newPrinter("core::private::LongArray", Core23ValueFieldArray)
    newPrinter("core::private::FloatArray", Core23ValueFieldArray)
    newPrinter("core::private::DoubleArray", Core23ValueFieldArray)
    newPrinter("core::util::AtomicIntegerArray", Core23ValueFieldArray)
    newPrinter("core::util::AtomicLongArray", Core23ValueFieldArray)

    newPrinter("core::String", Core23String)
    newPrinter("core::StringBuffer", Core23String)

    newPrinter("core::private::ReferenceArray", Core23RefFieldArray)
    newPrinter("core::private::AtomicReferenceArray", Core23RefFieldArray)
    newPrinter("core::util::Array", Core23RefFieldArray)

    newPrinter("core::util::ArrayList", Core23ArrayList)

    newPrinter("core::Trace", Core23Trace)
    newPrinter("core::util::PriorityQueue", Core23PriorityQueue)

    newPrinter("core::time::LocalDate", Core23LocalDate)
    newPrinter("core::time::LocalTime", Core23LocalTime)
    newPrinter("core::time::LocalDateTime", Core23LocalDateTime)


registerPrettyPrinters()
registerPrinters()
